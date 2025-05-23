/*
	This code was written by Micah Taylor, kixor.net, 2016 and is released into the Public Domain 
*/

#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <string.h>

// string_extra //
extern "C"
{
static char strequal(const char *s1, const char *s2);
static char contains(const char *haystack, const char *needle);

static char strequal(const char *s1, const char *s2)
{
	if(strcmp(s1, s2) == 0)
		return 1;
	return 0;
}

static char contains(const char *haystack, const char *needle)
{
	if(strstr(haystack, needle) == NULL)
		return 0;
	return 1;
}

}

// list //

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int item_count;
	int current_max_size;
	char growable;

	void **items;
	char **names;	
} list_;

extern "C"
{
	
static void list_make(list_ *listo, int size, char growable);
static int list_add_item(list_ *listo, void *item, char *name);
static char* list_print_items(list_ *listo);
static void* list_get_name(list_ *listo, char *name);
static void* list_get_index(list_ *listo, int indx);
static void* list_get_item(list_ *listo, void *item_to_find);
static int list_find(list_ *listo, char *name_to_find);
static void list_delete_index(list_ *listo, int indx);
static void list_delete_name(list_ *listo, char *name);
static void list_delete_item(list_ *listo, void *item);
static void list_delete_all(list_ *listo);
static void list_print_list(list_ *listo);
static void list_free(list_ *listo);

static void test_list();

// internal helper functions
static char list_is_full(list_ *listo)
{
	return(listo->item_count == listo->current_max_size);
}

static void list_grow(list_ *old_listo)
{
	int i;
	list_ new_listo;
	
	list_make(&new_listo, old_listo->current_max_size*2, old_listo->growable++);
	
	for(i=0; i<old_listo->current_max_size; i++)
		list_add_item(&new_listo, old_listo->items[i] , old_listo->names[i]);
	
	list_free(old_listo);
	
	//copy new structure to old list
	old_listo->names = new_listo.names;
	old_listo->items = new_listo.items;
	old_listo->item_count = new_listo.item_count;
	old_listo->current_max_size = new_listo.current_max_size;
	old_listo->growable = new_listo.growable;
}
//end helpers

static void list_make(list_ *listo, int start_size, char growable)
{
	listo->names = (char**) malloc(sizeof(char*) * start_size);
	listo->items = (void**) malloc(sizeof(void*) * start_size);
	listo->item_count = 0;
	listo->current_max_size = start_size;
	listo->growable = growable;
}

static int list_add_item(list_ *listo, void *item, char *name)
{
	int name_length;
	char *new_name;
	
	if( list_is_full(listo) )
	{
		if( listo->growable )
			list_grow(listo);
		else
			return -1;
	}
	
	listo->names[listo->item_count] = NULL;
	if(name != NULL)
	{
		name_length = strlen(name);
		new_name = (char*) malloc(sizeof(char) * name_length + 1);
		strncpy(new_name, name, name_length);
		listo->names[listo->item_count] = new_name;
	}
	
	listo->items[listo->item_count] = item;
	listo->item_count++;
	
	return listo->item_count-1;
}

static char* list_print_items(list_ *listo)
{
	int i;
	
	for(i=0; i < listo->item_count; i++)
	{
		printf("%s\n", listo->names[i]);
	}
	
	return NULL;
}

static void* list_get_index(list_ *listo, int indx)
{
	if(indx < listo->item_count)
		return listo->items[indx];
	return NULL;
}

static void* list_get_item(list_ *listo, void *item_to_find)
{
	int i = 0;
	
	for(i=0; i < listo->item_count; i++)
	{
		if(listo->items[i] == item_to_find)
			return listo->items[i];
	}
	
	return NULL;
}

static void* list_get_name(list_ *listo, char *name_to_find)
{
	int i = 0;
	
	for(i=0; i < listo->item_count; i++)
	{
		if(strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
			return listo->items[i];
	}
	
	return NULL;
}

static int list_find(list_ *listo, char *name_to_find)
{
	int i = 0;
	
	for(i=0; i < listo->item_count; i++)
	{
		if(strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
			return i;
	}
	
	return -1;
}

static void list_delete_item(list_ *listo, void *item)
{
	int i;
	
	for(i=0; i < listo->item_count; i++)
	{
		if( listo->items[i] == item )
			list_delete_index(listo, i);
	}
}

static void list_delete_name(list_ *listo, char *name)
{
	int i;
	//int j;
	//char remove = 0;
	
	//	int length_name = strlen(name);
	int item_name;
	
	if(name == NULL)
		return;
	
	for(i=0; i < listo->item_count; i++)
	{
		item_name = strlen(name);
		
		if( name != NULL && (strncmp(listo->names[i], name, strlen(name)) == 0) )
			list_delete_index(listo, i);
	}
}

static void list_delete_index(list_ *listo, int indx)
{
	int j;
	
	//remove item
	if(listo->names[indx] != NULL)
		free(listo->names[indx]);
	
	//restructure
	for(j=indx; j < listo->item_count-1; j++)
	{
		listo->names[j] = listo->names[j+1];
		listo->items[j] = listo->items[j+1];
	}
	
	listo->item_count--;
	
	return;
}

static void list_delete_all(list_ *listo)
{
	int i;
	
	for(i=listo->item_count-1; i>=0; i--)
		list_delete_index(listo, i);
}

static void list_free(list_ *listo)
{
	list_delete_all(listo);
	free(listo->names);
	free(listo->items);
}

static void list_print_list(list_ *listo)
{
	int i;
	
	printf("count: %i/%i\n", listo->item_count, listo->current_max_size);
	
	for(i=0; i < listo->item_count; i++)
	{
		printf("list[%i]: %s\n", i, listo->names[i]);
	}
}
	
}

// obj_parser //

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OBJ_FILENAME_LENGTH 500
#define MATERIAL_NAME_SIZE 255
#define OBJ_LINE_SIZE 500
#define MAX_VERTEX_COUNT 4 //can only handle quads or triangles
#define WHITESPACE " \t\n\r"

typedef struct obj_face
{
	int vertex_index[MAX_VERTEX_COUNT];
	int normal_index[MAX_VERTEX_COUNT];
	int texture_index[MAX_VERTEX_COUNT];
	int vertex_count;
	int material_index;
} obj_face;

typedef struct obj_sphere
{
	int pos_index;
	int up_normal_index;
	int equator_normal_index;
	int texture_index[MAX_VERTEX_COUNT];
	int material_index;
} obj_sphere;

typedef struct obj_plane
{
	int pos_index;
	int normal_index;
	int rotation_normal_index;
	int texture_index[MAX_VERTEX_COUNT];
	int material_index;
} obj_plane;

typedef struct obj_vector
{
	double e[3];
} obj_vector;

typedef struct obj_material
{
	char name[MATERIAL_NAME_SIZE];
	char texture_filename[OBJ_FILENAME_LENGTH];
	double amb[3];
	double diff[3];
	double spec[3];
	double reflect;
	double refract;
	double trans;
	double shiny;
	double glossy;
	double refract_index;
} obj_material;

typedef struct obj_camera
{
	int camera_pos_index;
	int camera_look_point_index;
	int camera_up_norm_index;
} obj_camera;

typedef struct obj_light_point
{
	int pos_index;
	int material_index;
} obj_light_point;

typedef struct obj_light_disc
{
	int pos_index;
	int normal_index;
	int material_index;
} obj_light_disc;

typedef struct obj_light_quad
{
	int vertex_index[MAX_VERTEX_COUNT];
	int material_index;
} obj_light_quad;

typedef struct obj_growable_scene_data
{
//	vector extreme_dimensions[2];
	char scene_filename[OBJ_FILENAME_LENGTH];
	char material_filename[OBJ_FILENAME_LENGTH];
	
	list_ vertex_list;
	list_ vertex_normal_list;
	list_ vertex_texture_list;
	
	list_ face_list;
	list_ sphere_list;
	list_ plane_list;
	
	list_ light_point_list;
	list_ light_quad_list;
	list_ light_disc_list;
	
	list_ material_list;
	
	obj_camera *camera;
} obj_growable_scene_data;

typedef struct obj_scene_data
{
	obj_vector **vertex_list;
	obj_vector **vertex_normal_list;
	obj_vector **vertex_texture_list;
	
	obj_face **face_list;
	obj_sphere **sphere_list;
	obj_plane **plane_list;
	
	obj_light_point **light_point_list;
	obj_light_quad **light_quad_list;
	obj_light_disc **light_disc_list;
	
	obj_material **material_list;
	
	int vertex_count;
	int vertex_normal_count;
	int vertex_texture_count;

	int face_count;
	int sphere_count;
	int plane_count;

	int light_point_count;
	int light_quad_count;
	int light_disc_count;

	int material_count;

	obj_camera *camera;
} obj_scene_data;

extern "C"
{
static int parse_obj_scene(obj_scene_data *data_out, const char *filename);
static void delete_obj_data(obj_scene_data *data_out);
}


static void obj_free_half_list(list_ *listo)
{
	list_delete_all(listo);
	free(listo->names);
}

static int obj_convert_to_list_index(int current_max, int index)
{
	if(index == 0)  //no index
		return -1;
	
	if(index < 0)  //relative to current list position
		return current_max + index;
	
	return index - 1;  //normal counting index
}

static void obj_convert_to_list_index_v(int current_max, int *indices)
{
	for(int i=0; i<MAX_VERTEX_COUNT; i++)
		indices[i] = obj_convert_to_list_index(current_max, indices[i]);
}

static void obj_set_material_defaults(obj_material *mtl)
{
	mtl->amb[0] = 0.2;
	mtl->amb[1] = 0.2;
	mtl->amb[2] = 0.2;
	mtl->diff[0] = 0.8;
	mtl->diff[1] = 0.8;
	mtl->diff[2] = 0.8;
	mtl->spec[0] = 1.0;
	mtl->spec[1] = 1.0;
	mtl->spec[2] = 1.0;
	mtl->reflect = 0.0;
	mtl->trans = 1;
	mtl->glossy = 98;
	mtl->shiny = 0;
	mtl->refract_index = 1;
	mtl->texture_filename[0] = '\0';
}

static int obj_parse_vertex_index(int *vertex_index, int *texture_index, int *normal_index)
{
	char *temp_str;
	char *token;
	int vertex_count = 0;
	
	
	while( (token = strtok(NULL, WHITESPACE)) != NULL)
	{
		if(texture_index != NULL)
			texture_index[vertex_count] = 0;
		if(normal_index != NULL)
			normal_index[vertex_count] = 0;
		
		vertex_index[vertex_count] = atoi( token );
		
		if(contains(token, "//"))  //normal only
		{
			temp_str = strchr(token, '/');
			temp_str++;
			normal_index[vertex_count] = atoi( ++temp_str );
		}
		else if(contains(token, "/"))
		{
			temp_str = strchr(token, '/');
			texture_index[vertex_count] = atoi( ++temp_str );
			
			if(contains(temp_str, "/"))
			{
				temp_str = strchr(temp_str, '/');
				normal_index[vertex_count] = atoi( ++temp_str );
			}
		}
		
		vertex_count++;
	}
	
	return vertex_count;
}

static obj_face* obj_parse_face(obj_growable_scene_data *scene)
{
	int vertex_count;
	obj_face *face = (obj_face*)malloc(sizeof(obj_face));
	
	vertex_count = obj_parse_vertex_index(face->vertex_index, face->texture_index, face->normal_index);
	obj_convert_to_list_index_v(scene->vertex_list.item_count, face->vertex_index);
	obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, face->texture_index);
	obj_convert_to_list_index_v(scene->vertex_normal_list.item_count, face->normal_index);
	face->vertex_count = vertex_count;
	
	return face;
}

static obj_sphere* obj_parse_sphere(obj_growable_scene_data *scene)
{
	int temp_indices[MAX_VERTEX_COUNT];
	
	obj_sphere *obj = (obj_sphere*)malloc(sizeof(obj_sphere));
	obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
	obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
	obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
	obj->up_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
	obj->equator_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);
	
	return obj;
}

static obj_plane* obj_parse_plane(obj_growable_scene_data *scene)
{
	int temp_indices[MAX_VERTEX_COUNT];
	
	obj_plane *obj = (obj_plane*)malloc(sizeof(obj_plane));
	obj_parse_vertex_index(temp_indices, obj->texture_index, NULL);
	obj_convert_to_list_index_v(scene->vertex_texture_list.item_count, obj->texture_index);
	obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
	obj->normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
	obj->rotation_normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[2]);
	
	return obj;
}

static obj_light_point* obj_parse_light_point(obj_growable_scene_data *scene)
{
	obj_light_point *o= (obj_light_point*)malloc(sizeof(obj_light_point));
	o->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, atoi( strtok(NULL, WHITESPACE)) );
	return o;
}

static obj_light_quad* obj_parse_light_quad(obj_growable_scene_data *scene)
{
	obj_light_quad *o = (obj_light_quad*)malloc(sizeof(obj_light_quad));
	obj_parse_vertex_index(o->vertex_index, NULL, NULL);
	obj_convert_to_list_index_v(scene->vertex_list.item_count, o->vertex_index);
	
	return o;
}

static obj_light_disc* obj_parse_light_disc(obj_growable_scene_data *scene)
{
	int temp_indices[MAX_VERTEX_COUNT];
	
	obj_light_disc *obj = (obj_light_disc*)malloc(sizeof(obj_light_disc));
	obj_parse_vertex_index(temp_indices, NULL, NULL);
	obj->pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, temp_indices[0]);
	obj->normal_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, temp_indices[1]);
	
	return obj;
}

static obj_vector* obj_parse_vector()
{
	obj_vector *v = (obj_vector*)malloc(sizeof(obj_vector));
	size_t const tokCount = 3;
	char const * tok;
	for(size_t i=0; i<tokCount; i++) {
		tok = strtok(NULL, WHITESPACE);
		if(tok != NULL)
			v->e[i] = atof( tok );
	}
	
	return v;
}

static void obj_parse_camera(obj_growable_scene_data *scene, obj_camera *camera)
{
	int indices[3];
	obj_parse_vertex_index(indices, NULL, NULL);
	camera->camera_pos_index = obj_convert_to_list_index(scene->vertex_list.item_count, indices[0]);
	camera->camera_look_point_index = obj_convert_to_list_index(scene->vertex_list.item_count, indices[1]);
	camera->camera_up_norm_index = obj_convert_to_list_index(scene->vertex_normal_list.item_count, indices[2]);
}

static int obj_parse_mtl_file(const char *filename, list_ *material_list)
{
	int line_number = 0;
	char *current_token;
	char current_line[OBJ_LINE_SIZE];
	char material_open = 0;
	obj_material *current_mtl = NULL;
	FILE *mtl_file_stream;
	
	// open scene
	mtl_file_stream = fopen( filename, "r");
	if(mtl_file_stream == 0)
	{
		fprintf(stderr, "Error reading file: %s\n", filename);
		return 0;
	}
	
	list_make(material_list, 10, 1);
	
	while( fgets(current_line, OBJ_LINE_SIZE, mtl_file_stream) )
	{
		current_token = strtok( current_line, " \t\n\r");
		line_number++;
		
		//skip comments
		if( current_token == NULL || strequal(current_token, "//") || strequal(current_token, "#"))
			continue;
		
		
		//start material
		else if( strequal(current_token, "newmtl"))
		{
			material_open = 1;
			current_mtl = (obj_material*) malloc(sizeof(obj_material));
			obj_set_material_defaults(current_mtl);
			
			// get the name
			strncpy(current_mtl->name, strtok(NULL, " \t"), MATERIAL_NAME_SIZE);
			list_add_item(material_list, current_mtl, current_mtl->name);
		}
		
		//ambient
		else if( strequal(current_token, "Ka") && material_open)
		{
			current_mtl->amb[0] = atof( strtok(NULL, " \t"));
			current_mtl->amb[1] = atof( strtok(NULL, " \t"));
			current_mtl->amb[2] = atof( strtok(NULL, " \t"));
		}
		
		//diff
		else if( strequal(current_token, "Kd") && material_open)
		{
			current_mtl->diff[0] = atof( strtok(NULL, " \t"));
			current_mtl->diff[1] = atof( strtok(NULL, " \t"));
			current_mtl->diff[2] = atof( strtok(NULL, " \t"));
		}
		
		//specular
		else if( strequal(current_token, "Ks") && material_open)
		{
			current_mtl->spec[0] = atof( strtok(NULL, " \t"));
			current_mtl->spec[1] = atof( strtok(NULL, " \t"));
			current_mtl->spec[2] = atof( strtok(NULL, " \t"));
		}
		//shiny
		else if( strequal(current_token, "Ns") && material_open)
		{
			current_mtl->shiny = atof( strtok(NULL, " \t"));
		}
		//transparent
		else if( strequal(current_token, "d") && material_open)
		{
			current_mtl->trans = atof( strtok(NULL, " \t"));
		}
		//reflection
		else if( strequal(current_token, "r") && material_open)
		{
			current_mtl->reflect = atof( strtok(NULL, " \t"));
		}
		//glossy
		else if( strequal(current_token, "sharpness") && material_open)
		{
			current_mtl->glossy = atof( strtok(NULL, " \t"));
		}
		//refract index
		else if( strequal(current_token, "Ni") && material_open)
		{
			current_mtl->refract_index = atof( strtok(NULL, " \t"));
		}
		// illumination type
		else if( strequal(current_token, "illum") && material_open)
		{
		}
		// texture map
		else if( strequal(current_token, "map_Kd") && material_open)
		{
			strncpy(current_mtl->texture_filename, strtok(NULL, " \t"), OBJ_FILENAME_LENGTH);
		}
		else
		{
			fprintf(stderr, "Unknown command '%s' in material file %s at line %i:\n\t%s\n",
					current_token, filename, line_number, current_line);
			//return 0;
		}
	}
	
	fclose(mtl_file_stream);
	
	return 1;
	
}

static int obj_parse_obj_file(obj_growable_scene_data *growable_data, const char *filename)
{
	FILE* obj_file_stream;
	int current_material = -1;
	char *current_token = NULL;
	char current_line[OBJ_LINE_SIZE];
	int line_number = 0;
	// open scene
	obj_file_stream = fopen( filename, "r");
	if(obj_file_stream == 0)
	{
		fprintf(stderr, "Error reading file: %s\n", filename);
		return 0;
	}
	
	char const * lastSlash = filename;
	while( NULL != strstr(lastSlash, "/"))
		lastSlash = strstr(lastSlash, "/")+1;
	size_t slashPos = lastSlash - filename;
	
	/*
	 extreme_dimensions[0].x = INFINITY; extreme_dimensions[0].y = INFINITY; extreme_dimensions[0].z = INFINITY;
	 extreme_dimensions[1].x = -INFINITY; extreme_dimensions[1].y = -INFINITY; extreme_dimensions[1].z = -INFINITY;
	 
	 if(v->x < extreme_dimensions[0].x) extreme_dimensions[0].x = v->x;
	 if(v->x > extreme_dimensions[1].x) extreme_dimensions[1].x = v->x;
	 if(v->y < extreme_dimensions[0].y) extreme_dimensions[0].y = v->y;
	 if(v->y > extreme_dimensions[1].y) extreme_dimensions[1].y = v->y;
	 if(v->z < extreme_dimensions[0].z) extreme_dimensions[0].z = v->z;
	 if(v->z > extreme_dimensions[1].z) extreme_dimensions[1].z = v->z;*/
	
	
	//parser loop
	while( fgets(current_line, OBJ_LINE_SIZE, obj_file_stream) )
	{
		current_token = strtok( current_line, " \t\n\r");
		line_number++;
		
		//skip comments
		if( current_token == NULL || current_token[0] == '#')
			continue;
		
		//parse objects
		else if( strequal(current_token, "v") ) //process vertex
		{
			list_add_item(&growable_data->vertex_list,  obj_parse_vector(), NULL);
		}
		
		else if( strequal(current_token, "vn") ) //process vertex normal
		{
			list_add_item(&growable_data->vertex_normal_list,  obj_parse_vector(), NULL);
		}
		
		else if( strequal(current_token, "vt") ) //process vertex texture
		{
			list_add_item(&growable_data->vertex_texture_list,  obj_parse_vector(), NULL);
		}
		
		else if( strequal(current_token, "f") ) //process face
		{
			obj_face *face = obj_parse_face(growable_data);
			face->material_index = current_material;
			list_add_item(&growable_data->face_list, face, NULL);
		}
		
		else if( strequal(current_token, "sp") ) //process sphere
		{
			obj_sphere *sphr = obj_parse_sphere(growable_data);
			sphr->material_index = current_material;
			list_add_item(&growable_data->sphere_list, sphr, NULL);
		}
		
		else if( strequal(current_token, "pl") ) //process plane
		{
			obj_plane *pl = obj_parse_plane(growable_data);
			pl->material_index = current_material;
			list_add_item(&growable_data->plane_list, pl, NULL);
		}
		
		else if( strequal(current_token, "p") ) //process point
		{
			//make a small sphere to represent the point?
		}
		
		else if( strequal(current_token, "lp") ) //light point source
		{
			obj_light_point *o = obj_parse_light_point(growable_data);
			o->material_index = current_material;
			list_add_item(&growable_data->light_point_list, o, NULL);
		}
		
		else if( strequal(current_token, "ld") ) //process light disc
		{
			obj_light_disc *o = obj_parse_light_disc(growable_data);
			o->material_index = current_material;
			list_add_item(&growable_data->light_disc_list, o, NULL);
		}
		
		else if( strequal(current_token, "lq") ) //process light quad
		{
			obj_light_quad *o = obj_parse_light_quad(growable_data);
			o->material_index = current_material;
			list_add_item(&growable_data->light_quad_list, o, NULL);
		}
		
		else if( strequal(current_token, "c") ) //camera
		{
			growable_data->camera = (obj_camera*) malloc(sizeof(obj_camera));
			obj_parse_camera(growable_data, growable_data->camera);
		}
		
		else if( strequal(current_token, "usemtl") ) // usemtl
		{
			current_material = list_find(&growable_data->material_list, strtok(NULL, WHITESPACE));
		}
		
		else if( strequal(current_token, "mtllib") ) // mtllib
		{
			strncpy(growable_data->material_filename, filename, slashPos);
			strncpy(growable_data->material_filename+slashPos, strtok(NULL, WHITESPACE), OBJ_FILENAME_LENGTH-slashPos);
			obj_parse_mtl_file(growable_data->material_filename, &growable_data->material_list);
			continue;
		}
		
		else if( strequal(current_token, "o") ) //object name
		{ }
		else if( strequal(current_token, "s") ) //smoothing
		{ }
		else if( strequal(current_token, "g") ) // group
		{ }
		
		else
		{
			printf("Unknown command '%s' in scene code at line %i: \"%s\".\n",
				   current_token, line_number, current_line);
		}
	}
	
	fclose(obj_file_stream);
	
	return 1;
}


static void obj_init_temp_storage(obj_growable_scene_data *growable_data)
{
	list_make(&growable_data->vertex_list, 10, 1);
	list_make(&growable_data->vertex_normal_list, 10, 1);
	list_make(&growable_data->vertex_texture_list, 10, 1);
	
	list_make(&growable_data->face_list, 10, 1);
	list_make(&growable_data->sphere_list, 10, 1);
	list_make(&growable_data->plane_list, 10, 1);
	
	list_make(&growable_data->light_point_list, 10, 1);
	list_make(&growable_data->light_quad_list, 10, 1);
	list_make(&growable_data->light_disc_list, 10, 1);
	
	list_make(&growable_data->material_list, 10, 1);
	
	growable_data->camera = NULL;
}

static void obj_free_temp_storage(obj_growable_scene_data *growable_data)
{
	obj_free_half_list(&growable_data->vertex_list);
	obj_free_half_list(&growable_data->vertex_normal_list);
	obj_free_half_list(&growable_data->vertex_texture_list);
	
	obj_free_half_list(&growable_data->face_list);
	obj_free_half_list(&growable_data->sphere_list);
	obj_free_half_list(&growable_data->plane_list);
	
	obj_free_half_list(&growable_data->light_point_list);
	obj_free_half_list(&growable_data->light_quad_list);
	obj_free_half_list(&growable_data->light_disc_list);
	
	obj_free_half_list(&growable_data->material_list);
}

static void delete_obj_data(obj_scene_data *data_out)
{
	int i;
	
	for(i=0; i<data_out->vertex_count; i++)
		free(data_out->vertex_list[i]);
	free(data_out->vertex_list);
	for(i=0; i<data_out->vertex_normal_count; i++)
		free(data_out->vertex_normal_list[i]);
	free(data_out->vertex_normal_list);
	for(i=0; i<data_out->vertex_texture_count; i++)
		free(data_out->vertex_texture_list[i]);
	free(data_out->vertex_texture_list);
	
	for(i=0; i<data_out->face_count; i++)
		free(data_out->face_list[i]);
	free(data_out->face_list);
	for(i=0; i<data_out->sphere_count; i++)
		free(data_out->sphere_list[i]);
	free(data_out->sphere_list);
	for(i=0; i<data_out->plane_count; i++)
		free(data_out->plane_list[i]);
	free(data_out->plane_list);
	
	for(i=0; i<data_out->light_point_count; i++)
		free(data_out->light_point_list[i]);
	free(data_out->light_point_list);
	for(i=0; i<data_out->light_disc_count; i++)
		free(data_out->light_disc_list[i]);
	free(data_out->light_disc_list);
	for(i=0; i<data_out->light_quad_count; i++)
		free(data_out->light_quad_list[i]);
	free(data_out->light_quad_list);
	
	for(i=0; i<data_out->material_count; i++)
		free(data_out->material_list[i]);
	free(data_out->material_list);
	
	free(data_out->camera);
}

static void obj_copy_to_out_storage(obj_scene_data *data_out, obj_growable_scene_data *growable_data)
{
	data_out->vertex_count = growable_data->vertex_list.item_count;
	data_out->vertex_normal_count = growable_data->vertex_normal_list.item_count;
	data_out->vertex_texture_count = growable_data->vertex_texture_list.item_count;
	
	data_out->face_count = growable_data->face_list.item_count;
	data_out->sphere_count = growable_data->sphere_list.item_count;
	data_out->plane_count = growable_data->plane_list.item_count;
	
	data_out->light_point_count = growable_data->light_point_list.item_count;
	data_out->light_disc_count = growable_data->light_disc_list.item_count;
	data_out->light_quad_count = growable_data->light_quad_list.item_count;
	
	data_out->material_count = growable_data->material_list.item_count;
	
	data_out->vertex_list = (obj_vector**)growable_data->vertex_list.items;
	data_out->vertex_normal_list = (obj_vector**)growable_data->vertex_normal_list.items;
	data_out->vertex_texture_list = (obj_vector**)growable_data->vertex_texture_list.items;
	
	data_out->face_list = (obj_face**)growable_data->face_list.items;
	data_out->sphere_list = (obj_sphere**)growable_data->sphere_list.items;
	data_out->plane_list = (obj_plane**)growable_data->plane_list.items;
	
	data_out->light_point_list = (obj_light_point**)growable_data->light_point_list.items;
	data_out->light_disc_list = (obj_light_disc**)growable_data->light_disc_list.items;
	data_out->light_quad_list = (obj_light_quad**)growable_data->light_quad_list.items;
	
	data_out->material_list = (obj_material**)growable_data->material_list.items;
	
	data_out->camera = growable_data->camera;
}

static int parse_obj_scene(obj_scene_data *data_out, const char *filename)
{
	obj_growable_scene_data growable_data;
	
	obj_init_temp_storage(&growable_data);
	if( obj_parse_obj_file(&growable_data, filename) == 0)
		return 0;
	
	//print_vector(NORMAL, "Max bounds are: ", &growable_data->extreme_dimensions[1]);
	//print_vector(NORMAL, "Min bounds are: ", &growable_data->extreme_dimensions[0]);
	
	obj_copy_to_out_storage(data_out, &growable_data);
	obj_free_temp_storage(&growable_data);
	return 1;
}



#endif
