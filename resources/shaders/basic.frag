#version 430

in vec2 uv;
flat in uint tex_id;

uniform sampler2DArray blockTextures;

void main() {
    gl_FragColor = texture(blockTextures, vec3(uv, tex_id));
}