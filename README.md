# Personal C Library

## Naming Standards

<p>In this project, standards are loosely inspired by GNOME's naming standards, which can be accessed in this <a href=https://developer.gnome.org/programming-guidelines/stable/namespacing.html.en>link</a>.</p>
<p><b>Macros:</b> e.g. ELF_DIE</p>
<p><b>Constants:</b> e.g. ELF_GRAPH_ORIENTED</p>
<p><b>Functions*:</b> e.g. say_hello()</p>
<p><b>Structs:</b> e.g. ElfGraph</p>
<p><b>Methods:</b> e.g. elfGraph_someActionToExecute()</p>
<p><b>Overloaded Methods:</b> e.g. elfGraph_someActionToExecute_overloadType()</p>
<p><b>File Names:</b> e.g. elf_graph.c</p>
<p></p>
<p><b> * Functions shouldn't be exported to outside of the file.</b></p>
<p><b>Never forget the most important rule: follow same standards as the code that surrounds the code you are adding.</b></p>

## Data Structures' Type and Implementation Ambiguity
It may happen that you will implement the same data structure for different specific types, or using different implementations. 

For example, you may implement a generic list for void pointers, and one for integers. Or you may implement a set structure using hashtables and then one using union-find algorithms.

For when these happen, use the following standards:

**[namespace]\[specific_type]\[specific_implementation]\[data_structure]**

**e.g. ElfIntHashSet**

You may omit the type or implementation for one of the ambiguous instances. The priority goes to the most generic, which should be void pointer.
