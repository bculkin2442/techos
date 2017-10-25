#ifndef TECHOS_PROCEXECUTE_H
#define TECHOS_PROCEXECUTE_H

/* 
 * Execute an image loaded from a file.
 *
 * Takes an offset into the image and returns:
 * - 0 if the process has finished executing
 * - The new offset into the image if it hasn't
 */
int executeimage(FILE *, int);
#endif
