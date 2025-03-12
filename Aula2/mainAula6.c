#include <stdio.h>
#include "vc.h"


//Abrir imagem em grayscale, fazer negativo e gravar em novo ficheiro
int main_vc0006(void)
{
	IVC* image, *imageHSV, *imageDST;

	image = vc_read_image("Images/HSV/HSVTestImage01.ppm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageHSV = vc_image_new(image->width, image->height, image->channels, image->levels);

	if (imageHSV == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}
	
	imageDST = vc_image_new(image->width, image->height, 1, image->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_rgb_to_hsv(image, imageHSV);

	//Intervalos dos hue(h), saturacao(s) e brilho(v)
	//hmin: 50
	//hmax: 70 -> amarelo
	//smin: 90
	//smax: 100
	//vmin: 95
	//vmax: 100
	vc_hsv_segmentation(imageHSV, imageDST, 50, 70, 90, 100, 95, 100);

	vc_write_image("vc0006.pgm", imageDST);

	vc_image_free(image);
	vc_image_free(imageHSV);
	vc_image_free(imageDST);



	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}