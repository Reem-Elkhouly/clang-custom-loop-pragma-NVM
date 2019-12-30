# Clang-Custom-Loop-Pragma-for-NVM

In this tutorial, we will go through how to add a custom clang loop pragma directive. In this implementation, we propose a loop pragma directive that marks a specific loop nest in the program. The information provided by the pragma will be attached as metadata to the outermost loop in the nest. According to the design of Clang (llvm-9.0.0 is used here), the metadata is actually attached to the back branch instruction in the loop that follows the pragma in the code. Our purpose is to use the programmer directive to help to insert instructions for the non-volatile main memory (NVM) data persistence. However, the same approach can be used to implement the pragma for any other purpose. To do that, we pass two pieces of information through the pragma; the name of the array that holds the critical data to be persisted and the specific loop to insert the instructions into marked by its induction variable. Please note that, according to your purpose, you may need only an ordinary pragma directive that attaches metadata to any instruction (not a loop pragma) then we recommend to read this [turorial](https://git.scc.kit.edu/CES/clang-custom-pragma).

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
