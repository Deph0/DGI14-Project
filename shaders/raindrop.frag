varying float intensity;

void main()
{
	vec4 color = gl_LightSource[0].diffuse * intensity;
	if (intensity < 0.01)
		color.a = 0.1;

	gl_FragColor = color;
}
