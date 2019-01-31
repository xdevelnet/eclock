/*
 * Copyright (c) 2018, Xdevelnet (xdevelnet at xdevelnet dot org)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#define _BSD_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iso646.h>
#include <fcgiapp.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "eclock.xdapp.c"

#define FASTCGI_CONFIG_SOCKET ":9898"

int lfd = STDERR_FILENO;

char *itoa(int value) {
	static char string[55];
	sprintf(string, "%d", value);
	return string;
}

void try_to_get_log_file() {
	int fd = open("eclock.app.log", O_WRONLY | O_CREAT | O_APPEND , 0644);
	if (fd >= 0) lfd = fd;
}

void error_thrower(const char *error) {
	size_t len = strlen(error);
	write(lfd, error, len);
}

int main() {
	try_to_get_log_file();
	int fcgifd = FCGX_OpenSocket(FASTCGI_CONFIG_SOCKET, 128);
	if (0 > fcgifd) return EXIT_FAILURE;
	//if (0 > chmod(FASTCGI_CONFIG_SOCKET, 0777)) return EXIT_FAILURE;
	if (FCGX_Init() < 0) error_thrower("fcgx_init failed!\n"), exit(EXIT_FAILURE);
	if (FCGX_InitRequest(&request, fcgifd, 0) < 0) error_thrower("fcgx_init_request failed!\n"), exit(EXIT_FAILURE);

	return app(EXIT_SUCCESS, EXIT_FAILURE); // what I was thinking when creating this?
}
