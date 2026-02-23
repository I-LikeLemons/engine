#include <GLES2/gl2.h>
#include <shader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>


void create_shader(Shader* shader, const char *vertex_path, const char *fragment_path) {
    FILE *vp;
    long v_size = 0;
    char* v_contents;

    vp = fopen(vertex_path, "rb");
    if (vp == NULL) {
	printf("failed to fidn shader\n");
	return;
    }

    fseek(vp, 0L, SEEK_END);
    v_size = ftell(vp) + 1;
    fclose(vp);

    vp = fopen(vertex_path, "r");
    v_contents = memset(malloc(v_size), '\0',v_size);
    fread(v_contents,1, v_size-1, vp);
    fclose(vp);


    
    FILE *fp;
    long f_size = 0;
    char* f_contents;

    fp = fopen(fragment_path, "rb");
    if (fp == NULL) {
	printf("failed to fidn shader\n");
	return;
    }

    fseek(fp, 0L, SEEK_END);
    f_size = ftell(fp) + 1;
    fclose(fp);

    fp = fopen(fragment_path, "r");
    f_contents = memset(malloc(f_size), '\0',f_size);
    fread(f_contents,1, f_size-1, fp);
    fclose(fp);


    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char**)&v_contents, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char**)&f_contents, NULL);
    glCompileShader(fragment);

    shader->id = glCreateProgram();
    glAttachShader(shader->id,vertex);
    glAttachShader(shader->id,fragment);
    glLinkProgram(shader->id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(f_contents);
    free(v_contents);
}
