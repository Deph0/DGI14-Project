varying float intensity;

void main()
{
	vec3 lightPos = vec3(0.0, 0.0, 10.0);
	vec3 lightDir = normalize(vec3(gl_Vertex) - lightPos);
	intensity = max(dot(lightDir, gl_Normal), 0.0);

	gl_Position = ftransform();
}
