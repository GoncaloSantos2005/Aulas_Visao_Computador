#include <stdio.h>
#include "vc.h"


//Converter imagem com escala de intensidades em cinzento numa imagem com uma escala em cores do espaço RGB
int main_vc0007(void)
{
	IVC* image, * imageHSV, * imageDST;

	image = vc_read_image("Images/FLIR/flir-02.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageDST = vc_image_new(image->width, image->height, 3, image->levels);

	if (imageDST == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_scale_gray_to_color_palette(image, imageDST);

	vc_write_image("vc0007.pgm", imageDST);

	vc_image_free(image);
	vc_image_free(imageDST);



	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}