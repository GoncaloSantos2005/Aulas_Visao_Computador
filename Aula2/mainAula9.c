#include <stdio.h>
#include "vc.h"

int main_vc0009(void) {
	IVC* image, * imageDST;

	image = vc_read_image("Images/Segm/cells.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageDST = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary(image, imageDST, 200);
	vc_gray_negative(imageDST);

	vc_write_image("vc-0009.pgm", imageDST);

	vc_image_free(image);
	vc_image_free(imageDST);




	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}

int main/*_vc0009_1*/(void) {
	IVC* image;

	image = vc_read_image("Images/Segm/cells.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary_global_mena(image);
	vc_gray_negative(image);

	vc_write_image("vc-0009_1.pgm", image);

	vc_image_free(image);

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}