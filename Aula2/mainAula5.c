#include <stdio.h>
#include "vc.h"


//Abrir imagem em grayscale, fazer negativo e gravar em novo ficheiro
int main_vc0005(void)
{
	IVC* image, *imageDST;

	image = vc_read_image("Images/HSV/HSVTestImage01.ppm");
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

	vc_rgb_to_hsv(image, imageDST);

	vc_write_image("vc0005.ppm", imageDST);

	vc_image_free(image);
	vc_image_free(imageDST);

	

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}