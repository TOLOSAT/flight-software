# TAPAS Documentation

TAPAS documentation is based on 2 tools: 
- README.md files included in each repository which is used to succinctly explain the utility of the repository. 
- Doxygen documentation which is done throughout the code. 

## README Instructions

There is nothing worse than opening a README and finding that it contains nothing but the repository name. This is why you should avoid producing this kind of situation. You should either :
- Don't have a README, if it is not needed then delete it.
- To have a README but this one brings explanations on the feature contained in the repository.
The only valid exception to having a README with nothing in it is when initializing a directory so that it is registered by git. 

## Doxygen Instructions

Doxygen is a wonderful tool for creating documentation from well-commented code. It is a particular syntax that after using a tool will generate documentation on your code. The doxygen website describes itself as :
> Doxygen can help you in three ways:
> - It can generate an on-line documentation browser (in HTML) and/or an off-line reference manual from a set of documented source files. There is also support for generating output in RTF (MS-Word), PostScript, hyperlinked PDF, compressed HTML, and Unix man pages. The documentation is extracted directly from the sources, which makes it much easier to keep the documentation consistent with the source code.
> - You can configure doxygen to extract the code structure from undocumented source files. This is very useful to quickly find your way in large source distributions. Doxygen can also visualize the relations between the various elements by means of include dependency graphs, inheritance diagrams, and collaboration diagrams, which are all generated automatically.
> - You can also use doxygen for creating normal documentation (as I did for the doxygen user manual and web-site). 

All doxygen comments are contained in 

    /** 
     * Place your comments here.
     */

or in the shorter form

    /** Place your comments here */

It is highly recommend that you use doxygen in at least the following 4 situations:
- Files declarations (header and source files)
- Function declarations
- Constant declarations (macro or global variable)
- Type declarations

To improve your doxygens comments we use the following tags :
- `@file` Indicates the name of the file (**required**). 
- `@author` Indicates who added the feature (**optional**)
- `@brief` Allows you to describe briefly what the commented element does (**required**).
- `@date` Indicates when the feature was added (**optional**)
- `@fn` Indicates the name of the function (**required**).
- `@param` Indicates the parameters of the function (**required**).
- `@retval` Indicates the return values of the functions (**required**).
And many others ... (cf. https://www.doxygen.nl/manual/commands.html)