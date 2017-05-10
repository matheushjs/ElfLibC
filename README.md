# Personal C Library

# Naming Standards

<p>In this project, standards are loosely inspired by GNOME's naming standards, which can be accessed in this <a href=https://developer.gnome.org/programming-guidelines/stable/namespacing.html.en>link</a>.</p>
<p><b>Macros:</b> e.g. ELF_DIE</p>
<p><b>Constants:</b> e.g. ELF_GRAPH_ORIENTED</p>
<p><b>Functions:</b> e.g. say_hello()</p>
<p><b>Structs:</b> e.g. ElfGraph</p>
<p><b>Methods:</b> e.g. elfGraph_someActionToExecute()</p>
<p><b>Overloaded Methods:</b> e.g. elfGraph_someActionToExecute_overloadType()</p>
<p><b>File Names:</b> e.g. elf_graph.c</p>
<p></p>
### Functions shouldn't be exported to outside of the file.
### Most important rule: follow same standards as the code that surrounds the code you are adding.
