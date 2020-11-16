// Copyright Richard Johnston 2020 <techpowerawaits@outlook.com>
//
// SPDX-license-identifier: 0BSD

/// @mainpage
/// Provides a simple method to crop a inital_str.
///
/// lcrop - how much to crop on left side; rcrop - how much to crop starting from null.
/// Usage: strcrop -l <left-crop> -s <inital_str> -r <right-crop>

/// @file strcrop.c

#define VERSION "1.1.0"

// Forces the use of insecure functions when using Microsoft's C Runtime Library.
// Removes certain deprecation warnings.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ARGS 7
#define REG_EXIT 0
#define USER_ERROR 1
#define MEM_ERROR 2
#define MAX_STR_LEN 180 ///> Only 180 chars supported in strings.
#define current_arg argv[count]

void print_usage();
void print_version();

int main(int argc, char **argv) {
	if ( argc > MAX_ARGS ) {
		print_usage();
		return(USER_ERROR);
	}
	char *inital_str; ///> The user-provided string.
	/// Allocate memory.
	inital_str = (char *) malloc(sizeof(char) * MAX_STR_LEN);

	if ( inital_str == NULL ) {
		fputs("FE: Unable to commit array to memory", stderr);
		return(MEM_ERROR);
	}

	int cleft_num = 0;
	int cright_num = 0;
	_Bool crop_left = false;
	_Bool crop_right = false;
	_Bool use_str = false;

	for ( int count = 1; count < argc; count++ ) {

		if ( strcmp(current_arg, "--version") == 0 ) {
			print_version();
			free(inital_str);
			return(REG_EXIT);
		}
		else if ( strcmp(current_arg, "--help") == 0 ) {
			print_usage();
			free(inital_str);
			return(REG_EXIT);
		}
		if ( strcmp(current_arg, "-l") == 0 ) {
			crop_left = true;
		}
		else if ( strcmp(current_arg, "-s") == 0 ) {
			use_str = true;
		}
		else if ( strcmp(current_arg, "-r") == 0 ) {
			crop_right = true;
		}
		else {
			if ( crop_left ) {
				cleft_num = atoi(current_arg);
				if ( cleft_num < 0 ) {
					fprintf(stderr, "\nFE: User provided value %i is negative", cleft_num);
					print_usage();
					free(inital_str);
					return(USER_ERROR);
				}
			}
			else if ( crop_right ) {
				cright_num = atoi(current_arg);
				if ( cright_num < 0 ) {
					fprintf(stderr, "\nFE: User provided value %i is negative", cleft_num);
					print_usage();
					free(inital_str);
					return(USER_ERROR);
				}
			}
			else if ( use_str ) {
				strncpy(inital_str, current_arg, MAX_STR_LEN);
			}
			else {
				print_usage();
				free(inital_str);
				return(USER_ERROR);
			}

			// Set bools back to zero.
			crop_left = false;
			crop_right = false;
			use_str = false;
		}
	}

	if (!strlen(inital_str)) {
		// If inital_str is non-existant
		fgets(inital_str, MAX_STR_LEN, stdin);
	}

	// Make sure to error out if crop values greater than string length.
	unsigned int inital_len = strlen(inital_str);
	unsigned int total_crop = cleft_num + cright_num;
	if (total_crop > inital_len) {
		fprintf(stderr, "FE: Total crop value %u is greater than length of string", \
			total_crop);
		free(inital_str);
		return(USER_ERROR);
	}
	// Crop left variables.
	unsigned int lcrop_len = inital_len - cleft_num;
	// Create left-cropped array.
	char *lcropped = (char *) malloc(sizeof(char) * lcrop_len);

	if ( lcropped == NULL ) {
		fputs("FE: Unable to commit array to memory", stderr);
		free(inital_str);
		return(MEM_ERROR);
	}
	unsigned int inital_elements = ++inital_len;
	for ( unsigned count = 0; count <= inital_elements; count++ ) {
		/// Add the left crop value onto the count value in order
		/// to ensure it is cropped.
		unsigned crop_count = count + cleft_num;
		lcropped[count] = inital_str[crop_count];
	}

	// Crop right variables
	unsigned int lcropped_len = strlen(lcropped);
	unsigned int f_len = lcropped_len - cright_num - 1; ///> Final character num.
	unsigned int f_elements = ++f_len; ///> Final elements length.
	/// A pointer to the final array with the right side cropped out.
    char *final_str = (char *) malloc(sizeof(char) * f_elements);

    if ( final_str == NULL ) {
		fputs("FE: Unable to commit array to memory", stderr);
		free(lcropped);
		free(inital_str);
    	return(MEM_ERROR);
    }

    /* Copy the lcropped[] array into the final_str[] array with the right-cropped characters removed. */
    /// Copies the correct elements of string while ensuring it is null-terminated.
    /// This is something that strncpy can't do.
    snprintf(final_str, ++f_elements, "%s", lcropped);
    printf("%s", final_str);
    free(lcropped);
    free(inital_str);
    free(final_str);
	return(REG_EXIT);
}

void print_usage() {
	puts("Usage: strcrop [-l <left-crop>] [-s <inital_str>] [-r <right-crop>] [--version] [--help]");
	puts("Please see appropriate documentation for details on how to use the program.");
}
void print_version() {
	printf("strcrop %s\n", VERSION);
	puts("Copyright 2020 Richard Johnston <techpowerawaits@outlook.com>");
	puts("SPDX-license-identifier: 0BSD");
	puts("This program is open source. Enjoy!");
}

