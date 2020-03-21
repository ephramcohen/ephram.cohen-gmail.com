// Read binary stl file and manipulate them */
//
//  Author: Ephram Cohen

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((__packed__)) {
	char name[80];
	uint32_t numberOfTriangles;
} Header;

typedef struct __attribute__((__packed__)) {
	float x;
	float y;
	float z;
} Point;

typedef struct __attribute__((__packed__)) {
	Point normal;
	Point vertex[3];
	uint16_t attribute;
} Vertex;

void vectSubtr(Point a, Point b, Point *res) {
	res->x = a.x - b.x;
	res->y = a.y - b.y;
	res->z = a.z - b.z;
}

void vectDotProd(Point a, Point b, Point *normal) {
	normal->x = a.y*b.z - a.z*b.y;
	normal->y = a.z*b.x - a.x*b.z;
	normal->z = a.x*b.y - a.y*b.x;
}

void minMax(float x, float *min, float *max) {
    if (x < *min)
    	*min = x;
    else if (*max < x)
    	*max = x;
}

void main (int n, char *cmd[]) {
	FILE *fIn, *fOut;
    Header hdr;
    Vertex *vert;

    float minx, maxx;
    float miny, maxy;
    float minz, maxz;

	fIn = fopen("All-Brand-Cap-Drain.stl", "rb");
	fOut = fopen("All-Brand-Cap-Drain_out.stl", "wb");

	// copy the header info to the output
	fread(&hdr, sizeof(hdr), 1, fIn);
	fwrite(&hdr, sizeof(hdr), 1, fOut);

	printf ("number of triangles = %d\n", hdr.numberOfTriangles);

// read the whole file because we will have to make two passes of the file.
	vert = malloc(hdr.numberOfTriangles * sizeof(Vertex));
	if (vert == NULL) {
		printf ("malloc failed\n");
		return;
	}
	fread(vert, sizeof(Vertex), hdr.numberOfTriangles, fIn);
    fclose(fIn);

// calc min & max x, y, and z even though we only need z
    minx = maxx = vert[0].vertex[0].x;
    miny = maxy = vert[0].vertex[0].y;
    minz = maxz = vert[0].vertex[0].z;
    for (int i=0; i<hdr.numberOfTriangles; i++) {
	 	for (int j=0; j<3; j++) {
		 	minMax(vert[i].vertex[j].x, &minx, &maxx);
		 	minMax(vert[i].vertex[j].y, &miny, &maxy);
		 	minMax(vert[i].vertex[j].z, &minz, &maxz);
		}
	}

	printf ("minx = %f, maxx = %f\n", minx, maxx);
	printf ("miny = %f, maxy = %f\n", miny, maxy);
	printf ("minz = %f, maxz = %f\n", minz, maxz);

    for (int i=0; i<hdr.numberOfTriangles; i++) {
	 	bool recalc_normal;

	 	recalc_normal = false;
	 	for (int j=0; j<3; j++) {

//	 		force minz to 0 so object stays flat during z scaling
		 	vert[i].vertex[j].z -= minz;

		 	if (vert[i].vertex[j].y <= -40) {
		 		vert[i].vertex[j].x *= 1 + 0.08;
		 		vert[i].vertex[j].z *= 1 + 0.08;
		 		recalc_normal = true;
		 	} else if (vert[i].vertex[j].y < -20) {
		 		float scale = 1 + 0.08 * (vert[i].vertex[j].y+20)/-20;
		 		vert[i].vertex[j].x *= scale;
		 		vert[i].vertex[j].z *= scale;
		 		recalc_normal = true;
		 	}
		}

		if (recalc_normal) {
			Point a, b;
			vectSubtr(vert[i].vertex[1], vert[i].vertex[0], &a);
			vectSubtr(vert[i].vertex[2], vert[i].vertex[0], &b);

			vectDotProd(a, b, &vert[i].normal);

			//ToDo:  rescale the normal to a unit normal
		}
	}

	fwrite(vert, sizeof(Vertex), hdr.numberOfTriangles, fOut);
	fclose (fOut);

	free (vert);
}
