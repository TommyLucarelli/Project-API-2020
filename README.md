# Project API (Project of Algorithms and Data structure) A.Y. 2019/20
The Algorithms and Data Structures project consists in the design and implementation of a C language program starting from a natural language specification of its operation. The structure of the specification is designed so that the candidate uses the knowledge acquired in the course of Algorithms and Principles of Informatics to achieve an implementation that is not only correct, but also with optimal efficiency.

The project falls within the scope of innovative teaching; the evaluation is made using tools used in computer science Olympics competitions. The objective of the project is to create a Text Editor with the basic functions (write, delete, change) but also it can perform unlimited UNDO and REDO. Everything has to be implemented respecting limits of memory and time.

## Implementation
In input, operations are provided by specifying type, line, and content. The system figures out what operation to perform by parsing the input and calls the appropriate function.
The text is saved in a dynamic array, while a double-pointer list is used to save the history of operations. To optimize the trade-off between memory and execution time, the system saves a snapshot of the text every 25 operations, so that even very large UNDOs perform efficiently.
