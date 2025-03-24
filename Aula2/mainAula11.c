#include <stdio.h>
#include "vc.h"


int main_vc00011(void) {
	IVC* image, * imageDST, *imageGRAY, *imageDST2, *imageEtoD;

	image = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageGRAY = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageGRAY == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	
	imageDST = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imageDST2 = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	
	imageEtoD = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary(image, imageGRAY, 127);
	vc_write_image("vc-00011_gray.pgm", imageGRAY);


	vc_binary_dilate(imageGRAY, imageDST, 3);
	vc_write_image("vc-00011_dilate.pgm", imageDST);


	vc_binary_erode(imageGRAY, imageDST2, 3);
	vc_write_image("vc-00011_erode.pgm", imageDST2);

	vc_binary_dilate(imageDST2, imageEtoD, 3);
	vc_write_image("vc-00011_erode_to_dilate.pgm", imageEtoD);

	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm");
	system("FilterGear vc-00011_gray.pgm"); 
	system("FilterGear vc-00011_dilate.pgm"); 
	system("FilterGear vc-00011_erode.pgm"); 
	system("FilterGear vc-00011_erode_to_dilate.pgm"); 

	vc_image_free(image);
	vc_image_free(imageGRAY);
	vc_image_free(imageDST);
	vc_image_free(imageDST2);
	vc_image_free(imageEtoD);

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}

int main_vc00011_1(void) {
	IVC* image, * imageERO, * imageGRAY, * imageDIL, *imageDIL2;

	image = vc_read_image("Images/Morfologicos/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageGRAY = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageGRAY == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imageERO = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageERO == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	imageDIL = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDIL == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	imageDIL2 = vc_image_new(imageGRAY->width, imageGRAY->height, imageGRAY->channels, imageGRAY->levels);

	if (imageDIL == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_gray_to_binary(image, imageGRAY, 127);
	vc_write_image("vc-00011_1_gray.pgm", imageGRAY);

	vc_binary_erode(imageGRAY, imageERO, 3);
	vc_write_image("vc-00011_1_erode.pgm", imageERO);

	vc_binary_dilate(imageERO, imageDIL, 3);
	vc_write_image("vc-00011_1_dilate.pgm", imageERO);	

	vc_binary_dilate(imageDIL, imageDIL2, 3);
	vc_write_image("vc-00011_1_dilate2.pgm", imageDIL2);


	system("cmd /c start FilterGear Images/Morfologicos/flir-01.pgm");
	system("FilterGear vc-00011_1_gray.pgm");
	system("FilterGear vc-00011_1_erode.pgm");
	system("FilterGear vc-00011_1_dilate.pgm");
	system("FilterGear vc-00011_1_dilate2.pgm");

	vc_image_free(image);
	vc_image_free(imageGRAY);
	vc_image_free(imageDIL);
	vc_image_free(imageERO);
	vc_image_free(imageDIL2);


	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}