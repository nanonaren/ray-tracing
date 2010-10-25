#include "ray_trace.h"

extern SceneInfo scene;

/*
*	read the information from the scene specification file
*	and place in data structure
*/

void readSceneFile(char *filename)
{
	FILE *inputfile;
	char type[7];
	int view = 0;
	int no_lights = 0;
	int no_spheres = 0;
	int no_polys = 0;
	int n;
	int i;

	inputfile = fopen(filename, "r");
	if(inputfile == NULL){
		printf("Cannot open specified input file.  Exiting.\n");
		exit(10);
	}

	while(fscanf(inputfile, "%s", &type) != EOF){
		if(strcmp(type, "VIEW") == 0){
			if(view == 0){
				fscanf(inputfile, "%f\n", &scene.view_z);
				view = 1;
			}
			else{
				printf("view specified twice...Exiting\n\n\n");
				exit(20);
			}
		}

		else if(strcmp(type, "LIGHT") == 0){
			if(no_lights < MAX_LIGHTS){
				fscanf(inputfile, "%f %f %f %f %f %f\n", &scene.lights[no_lights].x_pos,
					&scene.lights[no_lights].y_pos, &scene.lights[no_lights].z_pos, 
					&scene.lights[no_lights].red, &scene.lights[no_lights].green, 
					&scene.lights[no_lights].blue);
				no_lights++;
			}
			else{
				printf("too many lights specified...Exiting\n\n\n");
				exit(30);
			}
		}

		else if(strcmp(type, "POLY") == 0){
			if(no_polys < MAX_POLYS){
				fscanf(inputfile, "%d", &n);
				scene.polygons[no_polys].no_vertices = n;
				scene.polygons[no_polys].vertices =
					(Vertex *)malloc(sizeof(Vertex) * n);
				for(i=0; i<n; i++)
					fscanf(inputfile, "%f %f %f", 
						&scene.polygons[no_polys].vertices[i].x,
						&scene.polygons[no_polys].vertices[i].y,
						&scene.polygons[no_polys].vertices[i].z);
				fscanf(inputfile, "%f %f %f\n", 
					&scene.polygons[no_polys].red,
					&scene.polygons[no_polys].green,
					&scene.polygons[no_polys].blue);
				no_polys++;
			}
			else{
				printf("too many polygons specified...Exiting\n\n\n");
				exit(40);
			}
		}

		else if(strcmp(type, "SPHERE") == 0){
			if(no_spheres < MAX_SPHERES){
				fscanf(inputfile, "%f %f %f %f %f %f %f\n", 
					&scene.spheres[no_spheres].x_center,
					&scene.spheres[no_spheres].y_center,
					&scene.spheres[no_spheres].z_center,
					&scene.spheres[no_spheres].radius,
					&scene.spheres[no_spheres].red,
					&scene.spheres[no_spheres].green,
					&scene.spheres[no_spheres].blue);
				no_spheres++;
			}
			else{
				printf("too many spheres specified...Exiting\n\n\n");
				exit(50);
			}
		}
	}

	scene.no_lights = no_lights;
	scene.no_spheres = no_spheres;
	scene.no_polygons = no_polys;

	fclose(inputfile);
}
