# Clang-Custom-Loop-Pragma-for-NVM

In this tutorial, we will go through how to add a custom clang loop pragma directive. In this implementation, we propose a loop pragma directive that marks a specific loop nest in the program. The information provided by the pragma will be attached as metadata to the outermost loop in the nest. According to the design of Clang (llvm-9.0.0 is used here), the metadata is actually attached to the back branch instruction in the IR of the loop that follows the pragma in the code. Our purpose is to use the programmer directive to help to insert instructions for the non-volatile main memory (NVM) data persistence. However, the same approach can be used to implement the pragma for any other purpose. To do that, we pass two pieces of information through the pragma; the name of the array that holds the critical data to be persisted and the specific loop to insert the instructions into marked by its induction variable. Please note that, according to your purpose, you may need only an ordinary pragma directive that attaches metadata to any instruction (not a loop pragma) then we recommend to read this [turorial](https://git.scc.kit.edu/CES/clang-custom-pragma).

Through this tutorial, we assume that the reader already has some knowledge of the Calng and LLVM, if not please refer to the [LLVM Getting Started Docs](https://llvm.org/docs/GettingStarted.html). Adding a custom pragma is a much of work to do because the new keywords and the format of the clauses should be recognizable to the preprocessor. Then, the parser should be learned how to parse these newly introduced tokens. finally, the code generator should be modified to generate the metadata and attach them to the corresponding instructions in the IR. Of course, the program manipulation according to the custom pragma directive is a separate work to be done in an LLVM pass or a runtime library.

First, the keywords for the pragma clauses should be added to [include/clang/Basic/Attr.td](https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/a33747a2bb765be76190d0222865d71b9a1878ef/include/clang/Basic/Attr.td#L2954-L2972) as follows.



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


/lib/Parse/ParsePragma.cpp


/lib/Sema/SemaStmtAttr.cpp

/lib/Sema/SemaTemplateInstantiate.cpp

/lib/CodeGen/CGLoopInfo.cpp
