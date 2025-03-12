#include <stdio.h>
#include "vc.h"

int main_vc0008(void) {
	IVC* image, * imageHSV;

	image = vc_read_image("Images/PET/PET-Normal.ppm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageHSV = vc_image_new(image->width, image->height, 3, image->levels);

	printf("---- Normal Brain -----\n");
	vc_rgb_to_hsv(image, imageHSV);
	vce_brain_percentage(imageHSV);

	image = vc_read_image("Images/PET/PET-Alzheimer.ppm");
	if (image == NULL) {
		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		(void)getchar();
		return 0;
	}

	imageHSV = vc_image_new(image->width, image->height, 3, image->levels);

	printf("---- Alzheimer Brain -----\n");
	vc_rgb_to_hsv(image, imageHSV);
	vce_brain_percentage(imageHSV);



	vc_image_free(image);
	vc_image_free(imageHSV);
	



	printf("Press any key to exit...\n");
	(void)getchar();
	return 0;
}