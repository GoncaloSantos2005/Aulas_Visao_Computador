#include <stdio.h>
#include "vc.h"


//Abrir imagem em grayscale, fazer negativo e gravar em novo ficheiro
int main/*_vc0007*/(void)
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

}