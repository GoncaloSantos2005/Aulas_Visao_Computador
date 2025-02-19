#include <stdio.h>
#include "vc.h"


//Abrir imagem em grayscale, fazer negativo e gravar em novo ficheiro
int main_vc0004(void)
{
	IVC* image;

	image = vc_read_image("Images/FLIR/flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void) getchar();
		return 0;
	}

	vc_gray_negative(image);

	vc_write_image("vc0001.pgm", image);

	vc_image_free(image);

	//Windows
	system("cmd /c start FilterGear Images/FLIR/flir-01.pgm"); // Show input image
	system("FilterGear vc0001.pgm"); // Show output image

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}

//Abrir imagem em grayscale, fazer negativo e gravar em novo ficheiro
int main/*_vc0004_1*/(void)
{
	IVC* image[2];

	image[0] = vc_read_image("Images/Additional/fruits.ppm");
	if (image[0] == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	image[1] = vc_image_new(image[0]->width, image[0]->height, 1, image[0]->levels);
	if (image[1] == NULL) {
		printf("ERROR -> vc_image_new():\n\tFail to create file!\n");
		(void)getchar();
		return 0;
	}

	vc_rgb_to_gray(image[0], image[1]);

	vc_write_image("vc0004_1.pgm", image[1]);

	vc_image_free(image[0]);
	vc_image_free(image[1]);

	//Windows
	system("cmd /c start FilterGear Images/Additional/fruits.ppm"); // Show input image
	system("FilterGear vc0004_1.pgm"); // Show output image

	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}