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


//
// This file should contain only app-related stuff.
// It must have at least app() function to work, where we
// are going to place all logic.
// Usually it consists of infinity loop where(?) every iteration
// is the new request from client.
//
// Available API:
//
// XDAPP_COMPAT_ACCEPT; - must be called right after new loop iteration.
// XDAPP_COMPAT_FINISH; - must be called when we're done with client request(?).
// xdapp_write() - call this when you need to send output data.
// int app(int success, int failure); - here you describe the actual application.
//     This function should only return "success" or "failure" values.

#include <cdfel.c> // https://github.com/xdevelnet/cdfel
#include <time.h>
#include "webappconstants.h"

FCGX_Request request;

int writer_cb(void *buffer, unsigned long amount, void *aaa) {
	FCGX_PutStr(buffer, amount, request.out);
	return CDFEL_OK;
}

int hour_angle(int hours, int mins) {
	if (hours >= 12) hours-= 12; // 0 .. 24 to 0 .. 12
	return hours * 30 + mins / 2;
}

int mins_angle(int mins) {
	return mins * 6;
}

int app(int success, int failure) {
	CDFEL_INIT(rt, writer_cb);

	time_t rawtime;
	struct tm *timeinfo;

	while (1) {
		FCGX_Accept_r(&request);
		
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		int hangle = hour_angle(timeinfo->tm_hour, timeinfo->tm_min) - 90;
		int mangle = mins_angle(timeinfo->tm_min) - 90;

		CDFEL_RANGE(rt, pass_additional_headers, strizeof(pass_additional_headers));
		CDFEL_RANGE(rt, p1, strizeof(p1));
		CDFEL_SIGNED(rt, mangle);
		CDFEL_RANGE(rt, p2, strizeof(p2));
		CDFEL_SIGNED(rt, mangle);
		CDFEL_RANGE(rt, p3, strizeof(p3));
		CDFEL_SIGNED(rt, mangle);
		CDFEL_RANGE(rt, p4, strizeof(p4));
		CDFEL_SIGNED(rt, mangle);
		CDFEL_RANGE(rt, p5, strizeof(p5));
		CDFEL_SIGNED(rt, hangle);
		CDFEL_RANGE(rt, p6, strizeof(p6));
		CDFEL_SIGNED(rt, hangle);
		CDFEL_RANGE(rt, p7, strizeof(p7));
		CDFEL_SIGNED(rt, hangle);
		CDFEL_RANGE(rt, p8, strizeof(p8));
		CDFEL_SIGNED(rt, hangle);
		CDFEL_RANGE(rt, p9, strizeof(p9));
		CDFEL_SIGNED(rt, timeinfo->tm_hour);
		CDFEL_CSTRING(rt, ":");
		if (timeinfo->tm_min < 10) CDFEL_CSTRING(rt, "0");
		CDFEL_SIGNED(rt, timeinfo->tm_min);
		CDFEL_CSTRING(rt, ":");
		if (timeinfo->tm_sec < 10) CDFEL_CSTRING(rt, "0");
		CDFEL_SIGNED(rt, timeinfo->tm_sec);
		CDFEL_RANGE(rt, p10, strizeof(p10));
		CDFEL_DONE(rt);

		FCGX_Finish_r(&request);
	}

	return success;
}
