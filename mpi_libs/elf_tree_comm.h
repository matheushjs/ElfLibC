/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _ELF_TREE_COMM_H
#define _ELF_TREE_COMM_H

/* ELF TREE COMMUNICATION FUNCTIONS
 * 
 * The functions below try to make efficient use of the interconnection
 *   network of your system, by scattering/gathering data in a tree-like
 *   pattern.
 * 
 * All the functions below consider that the buffer to be scattered/gathered
 *   is in the node 0, and the content in the buffer is to be equally split
 *   among all other nodes.
 *
 *
 *
 * That is, consider you have 4 nodes, and node 0 has a buffer containing
 *   the characters:
 *
 *     char buf[] = "aabbccdd";
 * 
 * If you make the following call:
 *
 *     ElfTreeComm_scatter(buf, 2, MPI_CHAR, MPI_COMM_WORLD);
 *
 * you are telling our procedure that the buffer is to be considered as
 *   containing 4 pieces of data (because there are 4 nodes in the
 *   communicator, and each piece of data comprises 2 chars. Also, you
 *   are telling our procedure that the first piece of data should end
 *   in node 0, the second piece in node 1, and so on.
 *
 * Hence, the final buffer contents of each node would be:
 * 
 * node 0: "aa"
 * node 1: "bb"
 * node 2: "cc"
 * node 3: "dd"
 *
 * And the communication would happen in the following fashion:
 *
 * node 0 passes "ccdd" to node 2
 * node 0 passes "bb" to node 1
 * node 2 passes "dd" to node 3
 *
 * Which should be more efficient than when node 0 passes data to all other nodes,
 *   depending on the difference between the speed to transfer data and the speed
 *   to begin a communication with other nodes.
 *
 *
 * The gather operation is the inverse of the scatter operation.
 * Consider the buffer contents for all 4 nodes (same contents as after scattering):
 *
 * node 0: "aa"
 * node 1: "bb"
 * node 2: "cc"
 * node 3: "dd"
 * 
 * If you make the following call:
 *
 *     ElfTreeComm_gather(buf, 2, MPI_CHAR, MPI_COMM_WORLD);
 *
 * You are telling our procedure that 'buf' in node 0 should gather all the data
 *   elements, each being 2 chars. Also, the procedure considers that the data in
 *   node 0 should be the first element in the buffer, the data in node 1 the second
 *   element, and so on. Hence the final buffer content in node 0 would be:
 *
 * node 0: "aabbccdd"
 *
 * And the communication pattern would be:
 *
 * node 0 receives "bb" from node 1
 * node 2 receives "dd" from node 3
 * node 0 receives "ccdd" from node 2
 *
 */

#include <mpi/mpi.h>

void ElfTreeComm_scatter(void *buf, int sendCount, MPI_Datatype type, MPI_Comm comm);
void ElfTreeComm_gather(void *buf, int sendCount, MPI_Datatype type, MPI_Comm comm);

#endif
