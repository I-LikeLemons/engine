#ifndef SHADER_H
#define SHADER_H

typedef struct {
    int id;
}Shader;

void create_shader(Shader* shader, const char* vertex_path,const char* fragment_path);


#endif 
