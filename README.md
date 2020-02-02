# Clang-Custom-Loop-Pragma-for-NVM

In this tutorial, we will go through how to add a custom clang loop pragma directive. In this implementation, we propose a loop pragma directive that marks a specific loop nest in the program for the pupose of this [research](https://dl.acm.org/doi/10.1145/3371236). The information provided by the pragma will be attached as metadata to the outermost loop in the nest. According to the design of Clang (llvm-9.0.0 is used here), the metadata is actually attached to the back branch instruction in the IR of the loop that follows the pragma in the code. Our purpose is to use the programmer directive to help to insert instructions for the non-volatile main memory (NVM) data persistence. However, the same approach can be used to implement the pragma for any other purpose. To do that, we pass two pieces of information through the pragma; the name of the array that holds the critical data to be persisted and the specific loop to insert the instructions into marked by its induction variable. Please note that, according to your purpose, you may need only an ordinary pragma directive that attaches metadata to any instruction (not a loop pragma) then we recommend to read this [turorial](https://git.scc.kit.edu/CES/clang-custom-pragma).

Through this tutorial, we assume that the reader already has some knowledge of the Calng and LLVM, if not please refer to the [LLVM Getting Started Docs](https://llvm.org/docs/GettingStarted.html). Adding a custom pragma is a much of work to do because the new keywords and the format of the clauses should be recognizable to the preprocessor. Then, the parser should be learned how to parse these newly introduced tokens. finally, the code generator should be modified to generate the metadata and attach them to the corresponding instructions in the IR. Of course, the program manipulation according to the custom pragma directive is a separate work to be done in an LLVM pass or a runtime library.

Fortunately, Clang already implements loop pragmas such as unroll, vectorize, interleave, ... etc. These pragmas are defined through a LoopHint and they appear in the code in the following form for the unroll (that may take an integer parameter).
```
#pragma clang loop unroll
```
The custome pragma that we propose will have the following format
```
#pragma clang loop recompute (c ii)
```
Where c is the matrix name and ii is the specified loop induction variable. 

As the loop pragma already exists in Clang, we do not have to implement a completely new Attribute; we will extend the existing one to include the custome pragma. First, the keywords for the pragma clauses should be added to [include/clang/Basic/Attr.td](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/a33747a2bb765be76190d0222865d71b9a1878ef/include/clang/Basic/Attr.td#L2954-L2972) as follows.



```
  let Spellings = [Pragma<"clang", "loop">, Pragma<"", "unroll">,
                   Pragma<"", "nounroll">, Pragma<"", "unroll_and_jam">,
                   Pragma<"", "nounroll_and_jam">,
		   Pragma<"", "NVM">];///NVM
				      


  /// State of the loop optimization specified by the spelling.
  let Args = [EnumArgument<"Option", "OptionType",
                          ["vectorize", "vectorize_width", "interleave", "interleave_count",
                           "unroll", "unroll_count", "unroll_and_jam", "unroll_and_jam_count",
                           "pipeline", "pipeline_initiation_interval", "distribute", "recompute"],///NVM
                          ["Vectorize", "VectorizeWidth", "Interleave", "InterleaveCount",
                           "Unroll", "UnrollCount", "UnrollAndJam", "UnrollAndJamCount",
                           "PipelineDisabled", "PipelineInitiationInterval", "Distribute", "Recompute"]>,///NVM
              EnumArgument<"State", "LoopHintState",
                           ["enable", "disable", "numeric", "assume_safety", "full", "persist"],///NVM
                           ["Enable", "Disable", "Numeric", "AssumeSafety", "Full", "Persist"]>,///NVM
              ExprArgument<"Value">,
	      ExprArgument<"ValueExtra">];///NVM
```

[include/clang/Parse/LoopHint.h](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/944cb6cef0728816dff734072889054ca72204f4/include/clang/Parse/LoopHint.h#L38-L40)
```
  ///NVM
  Expr *ValueExprExtra;
  StringRef PersistStr;
```
  

include/clang/Parse/Parser.h dose not need any modifications because we use the existing loopHandler and we do not define any new handlers.
[struct LoopHint](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/944cb6cef0728816dff734072889054ca72204f4/include/clang/Parse/Parser.h#L40)


[handle method](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/944cb6cef0728816dff734072889054ca72204f4/include/clang/Parse/Parser.h#L730-L732)
```
  /// Handle the annotation token produced for
  /// #pragma clang loop and #pragma unroll.
  bool HandlePragmaLoopHint(LoopHint &Hint);
```

The definition of the mothod that parse the LoopHint pragma can be found here:
[Parse method](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/944cb6cef0728816dff734072889054ca72204f4/include/clang/Parse/Parser.h#L1957-L1960)
```
  StmtResult ParsePragmaLoopHint(StmtVector &Stmts,
                                 ParsedStmtContext StmtCtx,
                                 SourceLocation *TrailingElseLoc,
                                 ParsedAttributesWithRange &Attrs);
```

The newly introduced option of the pragma should be handled as follows, where the new code marked by ///NVM. All the extracted information from the pragma will be stored in the Hint struct.
[/lib/Parse/ParsePragma.cpp](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/4f6791a62711dac1048007110c7fb1bac26b7700/lib/Parse/ParsePragma.cpp#L1008-L1185)
```
static std::string PragmaLoopHintString(Token PragmaName, Token Option) {
  std::string PragmaString;


  if (Option.getIdentifierInfo()->getName() == "recompute") {
    PragmaString = "NVM recompute persist ";
  }
  ///NVM
  else if (PragmaName.getIdentifierInfo()->getName() == "loop") {
    PragmaString = "clang loop ";
    PragmaString += Option.getIdentifierInfo()->getName();
  } else if (PragmaName.getIdentifierInfo()->getName() == "unroll_and_jam") {
    PragmaString = "unroll_and_jam";
  } else {
    assert(PragmaName.getIdentifierInfo()->getName() == "unroll" &&
           "Unexpected pragma name");
    PragmaString = "unroll";
  }
  return PragmaString;
}


bool Parser::HandlePragmaLoopHint(LoopHint &Hint) {
  assert(Tok.is(tok::annot_pragma_loop_hint));
  PragmaLoopHintInfo *Info =
      static_cast<PragmaLoopHintInfo *>(Tok.getAnnotationValue());


  IdentifierInfo *PragmaNameInfo = Info->PragmaName.getIdentifierInfo();
  Hint.PragmaNameLoc = IdentifierLoc::create(
      Actions.Context, Info->PragmaName.getLocation(), PragmaNameInfo);


  // It is possible that the loop hint has no option identifier, such as
  // #pragma unroll(4).
  IdentifierInfo *OptionInfo = Info->Option.is(tok::identifier)
                                   ? Info->Option.getIdentifierInfo()
                                   : nullptr;
  Hint.OptionLoc = IdentifierLoc::create(
      Actions.Context, Info->Option.getLocation(), OptionInfo);


  llvm::ArrayRef<Token> Toks = Info->Toks;


  // Return a valid hint if pragma unroll or nounroll were specified
  // without an argument.
  bool PragmaUnroll = PragmaNameInfo->getName() == "unroll";
  bool PragmaNoUnroll = PragmaNameInfo->getName() == "nounroll";
  bool PragmaUnrollAndJam = PragmaNameInfo->getName() == "unroll_and_jam";
  bool PragmaNoUnrollAndJam = PragmaNameInfo->getName() == "nounroll_and_jam";
  if (Toks.empty() && (PragmaUnroll || PragmaNoUnroll || PragmaUnrollAndJam ||
                       PragmaNoUnrollAndJam)) {
    ConsumeAnnotationToken();
    Hint.Range = Info->PragmaName.getLocation();
    return true;
  }


  // The constant expression is always followed by an eof token, which increases
  // the TokSize by 1.
  assert(!Toks.empty() &&
         "PragmaLoopHintInfo::Toks must contain at least one token.");


  // If no option is specified the argument is assumed to be a constant expr.
  bool OptionUnroll = false;
  bool OptionUnrollAndJam = false;
  bool OptionDistribute = false;
  bool OptionPipelineDisabled = false;
  bool StateOption = false;


  bool OptionRecompute = false; ///NVM


  
  if (OptionInfo) { // Pragma Unroll does not specify an option.
    OptionUnroll = OptionInfo->isStr("unroll");
    OptionUnrollAndJam = OptionInfo->isStr("unroll_and_jam");
    OptionDistribute = OptionInfo->isStr("distribute");
    OptionPipelineDisabled = OptionInfo->isStr("pipeline");
    StateOption = llvm::StringSwitch<bool>(OptionInfo->getName())
                      .Case("vectorize", true)
                      .Case("interleave", true)
                      .Default(false) ||
                  OptionUnroll || OptionUnrollAndJam || OptionDistribute ||
                  OptionPipelineDisabled;
    
    OptionRecompute = OptionInfo->isStr("recompute");///NVM  
  }


  bool AssumeSafetyArg = !OptionUnroll && !OptionUnrollAndJam &&
    !OptionDistribute && !OptionPipelineDisabled && !OptionRecompute; ///NVM 
  // Verify loop hint has an argument.
  if (Toks[0].is(tok::eof)) {
    ConsumeAnnotationToken();
    Diag(Toks[0].getLocation(), diag::err_pragma_loop_missing_argument)
        << /*StateArgument=*/StateOption
        << /*FullKeyword=*/(OptionUnroll || OptionUnrollAndJam)
        << /*AssumeSafetyKeyword=*/AssumeSafetyArg<<OptionRecompute;///NVM 
    return false;
  }


  // Validate the argument.
  if (StateOption) {
    ConsumeAnnotationToken();
    SourceLocation StateLoc = Toks[0].getLocation();
    IdentifierInfo *StateInfo = Toks[0].getIdentifierInfo();


    bool Valid = OptionRecompute || StateInfo && ///NVM StateInfo &&
                 llvm::StringSwitch<bool>(StateInfo->getName())
                     .Case("disable", true)
                     .Case("enable", !OptionPipelineDisabled)
                     .Case("full", OptionUnroll || OptionUnrollAndJam)
                     .Case("assume_safety", AssumeSafetyArg)
                     .Case("persist", OptionRecompute) ///NVM 
                     .Default(false);
    if (!Valid) {
      if (OptionPipelineDisabled) {
        Diag(Toks[0].getLocation(), diag::err_pragma_pipeline_invalid_keyword);
      } else {
        Diag(Toks[0].getLocation(), diag::err_pragma_invalid_keyword)
            << /*FullKeyword=*/(OptionUnroll || OptionUnrollAndJam)
            << /*AssumeSafetyKeyword=*/AssumeSafetyArg;
      }
      return false;
    }
    if (Toks.size() > 2)
      Diag(Tok.getLocation(), diag::warn_pragma_extra_tokens_at_eol)
          << PragmaLoopHintString(Info->PragmaName, Info->Option);
    Hint.StateLoc = IdentifierLoc::create(Actions.Context, StateLoc, StateInfo);
  } else {
    // Enter constant expression including eof terminator into token stream.
    PP.EnterTokenStream(Toks, /*DisableMacroExpansion=*/false,
                        /*IsReinject=*/false);
    ConsumeAnnotationToken();


    ExprResult R;///NVM //= ParseConstantExpression();
    StringRef recomputeString; ///NVM




    if(OptionRecompute){///NVM
      recomputeString = Toks[0].getIdentifierInfo()->getName();
      ExprResult StringResult = ParseConstantExpression();
      ExprResult StringResultExtra = ParseConstantExpression();
      if(!StringResult.isInvalid()){
	Hint.ValueExpr = StringResult.get();
	Hint.ValueExprExtra = StringResultExtra.get();
      }
      //ConsumeToken();
    }
    else{
      //ExprResult R = ParseConstantExpression();
      R = ParseConstantExpression();
    }
    
    


    // Tokens following an error in an ill-formed constant expression will
    // remain in the token stream and must be removed.
    if (Tok.isNot(tok::eof)) {
      Diag(Tok.getLocation(), diag::warn_pragma_extra_tokens_at_eol)
          << PragmaLoopHintString(Info->PragmaName, Info->Option);
      while (Tok.isNot(tok::eof))
        ConsumeAnyToken();
    }


    ConsumeToken(); // Consume the constant expression eof terminator.


    if (!OptionRecompute && (R.isInvalid() ||
			     Actions.CheckLoopHintExpr(R.get(), Toks[0].getLocation())))///NVM
      return false;


    if(OptionRecompute){///NVM
      Hint.PersistStr = recomputeString;
    }
    else{
      
    // Argument is a constant expression with an integer type.
    Hint.ValueExpr = R.get();
    }
  }
  
  Hint.Range = SourceRange(Info->PragmaName.getLocation(),
                           Info->Toks.back().getLocation());
  return true;
}
```

Some work has to be done in 
[/lib/Sema/SemaStmtAttr.cpp](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/4f6791a62711dac1048007110c7fb1bac26b7700/lib/Sema/SemaStmtAttr.cpp#L1)
to extend the existing pragma clang loop to accomodate the custom pragma representation. Because the new pragma contains two arguments instead of one as the existing loop pragmas, we need to tell the parser to look for an [additional token](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/4f6791a62711dac1048007110c7fb1bac26b7700/lib/Sema/SemaStmtAttr.cpp#L85).
```
  Expr *ValueExprExtra = A.getArgAsExpr(4);///NVM  
```


```
static Attr *handleLoopHintAttr(Sema &S, Stmt *St, const ParsedAttr &A,
                                SourceRange) {
  IdentifierLoc *PragmaNameLoc = A.getArgAsIdent(0);
  IdentifierLoc *OptionLoc = A.getArgAsIdent(1);
  IdentifierLoc *StateLoc = A.getArgAsIdent(2);
  Expr *ValueExpr = A.getArgAsExpr(3);


  Expr *ValueExprExtra = A.getArgAsExpr(4);///NVM  


  bool PragmaUnroll = PragmaNameLoc->Ident->getName() == "unroll";
  bool PragmaNoUnroll = PragmaNameLoc->Ident->getName() == "nounroll";
  bool PragmaUnrollAndJam = PragmaNameLoc->Ident->getName() == "unroll_and_jam";
  bool PragmaNoUnrollAndJam =
      PragmaNameLoc->Ident->getName() == "nounroll_and_jam";
  bool PragmaNVM = false;///NVM


  if(!PragmaUnroll && !PragmaNoUnroll && !PragmaUnrollAndJam && !PragmaNoUnrollAndJam)
    PragmaNVM = OptionLoc->Ident->getName() == "recompute";///////NVM


  
  if (St->getStmtClass() != Stmt::DoStmtClass &&
      St->getStmtClass() != Stmt::ForStmtClass &&
      St->getStmtClass() != Stmt::CXXForRangeStmtClass &&
      St->getStmtClass() != Stmt::WhileStmtClass) {
    const char *Pragma =
        llvm::StringSwitch<const char *>(PragmaNameLoc->Ident->getName())
            .Case("unroll", "#pragma unroll")
            .Case("nounroll", "#pragma nounroll")
            .Case("unroll_and_jam", "#pragma unroll_and_jam")
            .Case("nounroll_and_jam", "#pragma nounroll_and_jam")
            .Case("NVM", "#pragma NVM")///NVM 
            .Default("#pragma clang loop");
    S.Diag(St->getBeginLoc(), diag::err_pragma_loop_precedes_nonloop) << Pragma;
    return nullptr;
  }


  LoopHintAttr::Spelling Spelling =
      LoopHintAttr::Spelling(A.getAttributeSpellingListIndex());
  LoopHintAttr::OptionType Option;
  LoopHintAttr::LoopHintState State;


  if(PragmaNVM){///NVM
    //#pragma NVM recompute
    Option = LoopHintAttr::Recompute;
    State = LoopHintAttr::Persist;
  }
  else if (PragmaNoUnroll) {
    // #pragma nounroll
    Option = LoopHintAttr::Unroll;
    State = LoopHintAttr::Disable;
  } else if (PragmaUnroll) {
    if (ValueExpr) {
      // #pragma unroll N
      Option = LoopHintAttr::UnrollCount;
      State = LoopHintAttr::Numeric;
    } else {
      // #pragma unroll
      Option = LoopHintAttr::Unroll;
      State = LoopHintAttr::Enable;
    }
  } else if (PragmaNoUnrollAndJam) {
    // #pragma nounroll_and_jam
    Option = LoopHintAttr::UnrollAndJam;
    State = LoopHintAttr::Disable;
  } else if (PragmaUnrollAndJam) {
    if (ValueExpr) {
      // #pragma unroll_and_jam N
      Option = LoopHintAttr::UnrollAndJamCount;
      State = LoopHintAttr::Numeric;
    } else {
      // #pragma unroll_and_jam
      Option = LoopHintAttr::UnrollAndJam;
      State = LoopHintAttr::Enable;
    }
  } else {
    // #pragma clang loop ...
    assert(OptionLoc && OptionLoc->Ident &&
           "Attribute must have valid option info.");
    Option = llvm::StringSwitch<LoopHintAttr::OptionType>(
                 OptionLoc->Ident->getName())
                 .Case("vectorize", LoopHintAttr::Vectorize)
                 .Case("vectorize_width", LoopHintAttr::VectorizeWidth)
                 .Case("interleave", LoopHintAttr::Interleave)
                 .Case("interleave_count", LoopHintAttr::InterleaveCount)
                 .Case("unroll", LoopHintAttr::Unroll)
                 .Case("unroll_count", LoopHintAttr::UnrollCount)
                 .Case("pipeline", LoopHintAttr::PipelineDisabled)
                 .Case("pipeline_initiation_interval",
                       LoopHintAttr::PipelineInitiationInterval)
                 .Case("distribute", LoopHintAttr::Distribute)
                 .Case("recompute", LoopHintAttr::Recompute)///NVM  
                 .Default(LoopHintAttr::Vectorize);
    if (Option == LoopHintAttr::VectorizeWidth ||
        Option == LoopHintAttr::InterleaveCount ||
        Option == LoopHintAttr::UnrollCount ||
        Option == LoopHintAttr::PipelineInitiationInterval ||
	Option == LoopHintAttr::Recompute) {///NVM 
      assert(ValueExpr && "Attribute must have a valid value expression.");
      if (S.CheckLoopHintExpr(ValueExpr, St->getBeginLoc()))
        return nullptr;
      if (Option == LoopHintAttr::Recompute)///NVM
	State = LoopHintAttr::Persist;
      else
	State = LoopHintAttr::Numeric;
    } else if (Option == LoopHintAttr::Vectorize ||
               Option == LoopHintAttr::Interleave ||
               Option == LoopHintAttr::Unroll ||
               Option == LoopHintAttr::Distribute ||
               Option == LoopHintAttr::PipelineDisabled) {
      assert(StateLoc && StateLoc->Ident && "Loop hint must have an argument");
      if (StateLoc->Ident->isStr("disable"))
        State = LoopHintAttr::Disable;
      else if (StateLoc->Ident->isStr("assume_safety"))
        State = LoopHintAttr::AssumeSafety;
      else if (StateLoc->Ident->isStr("full"))
        State = LoopHintAttr::Full;
      else if (StateLoc->Ident->isStr("enable"))
        State = LoopHintAttr::Enable;
      else
        llvm_unreachable("bad loop hint argument");
    } else
      llvm_unreachable("bad loop hint");
  }


  return LoopHintAttr::CreateImplicit(S.Context, Spelling, Option, State,
                                      ValueExpr, ValueExprExtra, A.getRange());///NVM
}


static void
CheckForIncompatibleAttributes(Sema &S,
                               const SmallVectorImpl<const Attr *> &Attrs) {
  // There are 6 categories of loop hints attributes: vectorize, interleave,
  // unroll, unroll_and_jam, pipeline and distribute. Except for distribute they
  // come in two variants: a state form and a numeric form.  The state form
  // selectively defaults/enables/disables the transformation for the loop
  // (for unroll, default indicates full unrolling rather than enabling the
  // transformation). The numeric form form provides an integer hint (for
  // example, unroll count) to the transformer. The following array accumulates
  // the hints encountered while iterating through the attributes to check for
  // compatibility.
  struct {
    const LoopHintAttr *StateAttr;
    const LoopHintAttr *NumericAttr;
    const LoopHintAttr *StringAttr;///NVM
    
  } HintAttrs[] = {{nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr},
                   {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr},
		   {nullptr, nullptr}};///NVM


  for (const auto *I : Attrs) {
    const LoopHintAttr *LH = dyn_cast<LoopHintAttr>(I);


    // Skip non loop hint attributes
    if (!LH)
      continue;


    LoopHintAttr::OptionType Option = LH->getOption();
    enum {
      Vectorize,
      Interleave,
      Unroll,
      UnrollAndJam,
      Distribute,
      Pipeline,
      Recompute/*////NVM*/
    } Category;
    switch (Option) {
    case LoopHintAttr::Vectorize:
    case LoopHintAttr::VectorizeWidth:
      Category = Vectorize;
      break;
    case LoopHintAttr::Interleave:
    case LoopHintAttr::InterleaveCount:
      Category = Interleave;
      break;
    case LoopHintAttr::Unroll:
    case LoopHintAttr::UnrollCount:
      Category = Unroll;
      break;
    case LoopHintAttr::UnrollAndJam:
    case LoopHintAttr::UnrollAndJamCount:
      Category = UnrollAndJam;
      break;
    case LoopHintAttr::Distribute:
      // Perform the check for duplicated 'distribute' hints.
      Category = Distribute;
      break;
    case LoopHintAttr::PipelineDisabled:
    case LoopHintAttr::PipelineInitiationInterval:
      Category = Pipeline;
      break;
    case LoopHintAttr::Recompute:///NVM
      Category = Recompute;
      break;
      
    };


    assert(Category < sizeof(HintAttrs) / sizeof(HintAttrs[0]));
    auto &CategoryState = HintAttrs[Category];
    const LoopHintAttr *PrevAttr;
    if (Option == LoopHintAttr::Vectorize ||
        Option == LoopHintAttr::Interleave || Option == LoopHintAttr::Unroll ||
        Option == LoopHintAttr::UnrollAndJam ||
        Option == LoopHintAttr::PipelineDisabled ||
        Option == LoopHintAttr::Distribute) {
      // Enable|Disable|AssumeSafety hint.  For example, vectorize(enable).
      PrevAttr = CategoryState.StateAttr;
      CategoryState.StateAttr = LH;
    }


    else if ( Option == LoopHintAttr::Recompute ) {///NVM
      PrevAttr = CategoryState.StringAttr;
      CategoryState.StringAttr = LH;
    }
    


    else {
      // Numeric hint.  For example, vectorize_width(8).
      PrevAttr = CategoryState.NumericAttr;
      CategoryState.NumericAttr = LH;
    }


    PrintingPolicy Policy(S.Context.getLangOpts());
    SourceLocation OptionLoc = LH->getRange().getBegin();
    if (PrevAttr)
      // Cannot specify same type of attribute twice.
      S.Diag(OptionLoc, diag::err_pragma_loop_compatibility)
          << /*Duplicate=*/true << PrevAttr->getDiagnosticName(Policy)
          << LH->getDiagnosticName(Policy);


    if (CategoryState.StateAttr && CategoryState.NumericAttr &&
        (Category == Unroll || Category == UnrollAndJam ||
         CategoryState.StateAttr->getState() == LoopHintAttr::Disable)) {
      // Disable hints are not compatible with numeric hints of the same
      // category.  As a special case, numeric unroll hints are also not
      // compatible with enable or full form of the unroll pragma because these
      // directives indicate full unrolling.
      S.Diag(OptionLoc, diag::err_pragma_loop_compatibility)
          << /*Duplicate=*/false
          << CategoryState.StateAttr->getDiagnosticName(Policy)
          << CategoryState.NumericAttr->getDiagnosticName(Policy);
    }
  }
}
```
The call to the CreateImplicit should be modified to accomodate the newly added parameters in
[/lib/Sema/SemaTemplateInstantiate.cpp](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/c3a189a0ddc3c33994f63e79b4f56e29a7be04d0/lib/Sema/SemaTemplateInstantiate.cpp#L1255-L1257)

```
  return LoopHintAttr::CreateImplicit(
      getSema().Context, LH->getSemanticSpelling(), LH->getOption(),
      LH->getState(), TransformedExpr, nullptr, LH->getRange());///NVM
```

As the pragma was parced and handled, it is the time to start creating the metadata that to be attached to the specified loop.
[/lib/CodeGen/CGLoopInfo.cpp](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/c3a189a0ddc3c33994f63e79b4f56e29a7be04d0/lib/CodeGen/CGLoopInfo.cpp#L1)
here is where the metadata is written and attached as loop info.
```
MDNode *LoopInfo::createRecomputeMetadata(
					const LoopAttributes &Attrs,
					ArrayRef<Metadata *> LoopProperties,
					bool &HasUserTransforms
					){


  LLVMContext &Ctx = Header->getContext();

 
  if (Attrs.StringExpr.empty())
      return createFullUnrollMetadata(Attrs, LoopProperties, HasUserTransforms);


  //////////////////The current implementation does not follow up
  //////////////////with other transformations after persistence if found
    
  SmallVector<Metadata *, 4> Args;
  TempMDTuple TempNode = MDNode::getTemporary(Ctx, None);
  Args.push_back(TempNode.get());
  Args.append(LoopProperties.begin(), LoopProperties.end());
  


  Metadata *Vals[] = {MDString::get(Ctx, "llvm.NVM.recompute.persist"),
			  MDString::get(Ctx, Attrs.StringExpr)
      };
      Args.push_back(MDNode::get(Ctx, Vals));


      
  MDNode *LoopID = MDNode::getDistinct(Ctx, Args);
  LoopID->replaceOperandWith(0, LoopID);
  HasUserTransforms = true;
  return LoopID;
  
}
```
Note that you will have to include the proper header files.

By now the metadata is created, attached to the loop and will appear in the IR code as shown in the following example. These metadata should be read in the back end to make transformation and optimizaion decisions at code generation.


###Example

For the following C code from tmm.c
```
int tmm(){
  double sum=0; int ii, jj, kk, i, j, k;

  #pragma clang loop recompute (c ii)
  for (kk=0; kk<SIZE; kk=kk+bsize){
    for ( ii =0; ii <SIZE; ii=ii+bsize){
      for ( jj =0; jj <SIZE; jj=jj+bsize){
        for ( i =ii ; i <(ii+bsize); i=i+1){
          for ( j=jj ; j<(jj+bsize); j=j+1)
            {
              sum = c[i ][ j ];
              for (k=kk; k<(kk+bsize); k=k+1){
                sum += a[i][k]*b[k ][ j ];
              }
              c[ i ][ j ] = sum;
            } //end of j loop
        }//end of i loop
      }//end of jj loop
      //HERE: insertion point
    }//end of ii loop
  }//end of kk loop

  return 0;
}
```
The IR code can be generated using the command line
```
clang -S -emit-llvm tmm.c -o tmm.ll
```
The resulting IR code will  look like this. Where the metadata appears at the end of the file.
```
!2 = distinct !{!2, !3}
!3 = !{!"llvm.NVM.recompute.persist", !"c ii"}
```
