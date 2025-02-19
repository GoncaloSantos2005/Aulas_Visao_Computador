#include "stdio.h"
#include "vc.h"

//Teste Aula
//Criar uma imagem binária e gravar para ficheiro 
int main_vc0003_1(void)
{
	IVC* image;
	int x, y;
	long int pos;

	image = vc_image_new(256, 256, 1, 255);
	if (image == NULL) {
		printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
		getchar();
		return 0;
	}

	for (x = 0; x < image->width; x++) {
		for (y = 0; y < image->height; y++) {
			pos = y * image->bytesperline + x * image->channels;
		
			//image->data[pos] = y; //degrade cima baixo
			//image->data[pos] = x; //degrade esq direita
			//image->data[pos] = (unsigned char) (((float) x/ (float)x/ 
			//image->data[pos] = (unsigned)(x + y) / 2; //degrade diagnoal 
		}
	}

	vc_write_image("teste.pbm", image);

	vc_image_free(image);

	printf("Press any key to exit...\n");
	getchar();

	return 0;
}
