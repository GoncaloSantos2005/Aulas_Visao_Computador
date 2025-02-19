#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main_vc0002(void)
{
	IVC* image;
	int i;

	image = vc_read_image("C:\Temp\Visão por Computador\Images\FLIR\flir-01.pgm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void) getchar();
		return 0;
	}

	for (i = 0; i < image->bytesperline * image->height; i += image->channels) {
		image->data[i] = 255 - image->data[i];
	}

	vc_write_image("vc-0001.pgm", image);

	vc_image_free(image);

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}