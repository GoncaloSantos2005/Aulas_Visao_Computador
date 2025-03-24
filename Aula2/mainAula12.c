#include <stdio.h>
#include "vc.h"


int main_vc00012(void) {
	IVC* image,*image2, * imageDST;

	image = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image2 = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (image2 == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	
	imageDST = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	vc_gray_to_binary(image, image2, 128);
	vc_binary_open(image2, imageDST, 3, 3);
	vc_write_image("vc-00012.pgm", imageDST);

	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm");
	system("FilterGear vc-00012.pgm");

	vc_image_free(image);
	vc_image_free(image2);
	vc_image_free(imageDST);

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}

int main/*_vc00012_ex2*/(void)
{
	IVC* image, * imageDST, * image1, * image2, *image3, * image4;

	image = vc_read_image("Images/EX2/brain.pgm");
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
	
	image1 = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (image1 == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	image2 = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (image2 == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	image3 = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (image3 == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	image4 = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (image4 == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	vc_gray_to_binary_(image, image1, 60, 205);
	vc_binary_open(image1, image2, 7, 5);
	vc_binary_close(image2, image3, 7, 7);
	vc_binary_open(image3, image4, 7, 5);
	vc_write_image("vc-00012_ex2.pgm", image4);

	vc_write_image_binary_to_gray(image, image4, imageDST);
	vc_write_image("vc-00012_dst.pgm", imageDST);

	system("cmd /c start FilterGear Images/EX2/brain.pgm");
	system("cmd /c start FilterGear vc-00012_ex2.pgm");
	system("cmd /c start FilterGear vc-00012_dst.pgm");

	vc_image_free(image);
	vc_image_free(imageDST);
	vc_image_free(image1);
	vc_image_free(image2);
	vc_image_free(image3);
	vc_image_free(image4);

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}