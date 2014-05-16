varying float intensity;

void main()
{
	vec3 lightPos = vec3(0.2, -10.0, 0.0);
	vec3 lightDir = normalize(vec3(gl_Vertex) - lightPos);
	intensity = dot(lightDir, gl_Normal);

	gl_Position = ftransform();
}
