#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

// CPU to shader data GPU 
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D texture_diffuse1;

void main()
{
    // 1. Displays the submitted texture
    vec4 texElement = texture(texture_diffuse1, TexCoords);
    
    // 2. If the texture is the neutral white one (or if the model lacks an actual diffuse texture),
    // multiplying by material.diffuse preserves the editor color.
    // If it is an actual colored texture, it will blend with the material's base color.
    vec3 baseDiffuse = material.diffuse * texElement.rgb;
    vec3 baseAmbient = material.ambient * texElement.rgb;

    // ambient (Illuminates the calculated color with ambient light)
    vec3 ambient = light.ambient * baseAmbient;
  	
    // diffuse (Applies diffuse light to the combined color)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * baseDiffuse);
    
    // specular (Retains the material's original sheen)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 