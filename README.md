# Clang-Custom-Loop-Pragma-for-NVM
# clang-custom-loop-pragma-NVM

This is how to add a custom clang loop pragma directive.

First, the keywords for the pragma clauses should be added to include/clang/Basic/Attr.td as follows.

https://github.com/Reem-Elkhouly/clang-custom-loop-pragma-NVM/blob/a33747a2bb765be76190d0222865d71b9a1878ef/include/clang/Basic/Attr.td#L2954-L2972
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
