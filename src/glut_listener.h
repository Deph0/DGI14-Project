#ifndef GLUT_LISTENER_H
#define GLUT_LISTENER_H

class GlutListener {
public:
	template<int I> struct X { enum { TYPE = I }; };
	typedef X<0> Initialize;
	typedef X<1> Reshape;
	typedef X<2> Display;
	typedef X<3> Idle;
	typedef X<4> KeyDown;
	typedef X<5> SpecialKeyDown;
	typedef X<6> MouseButton;
	typedef X<7> MouseMove;

	virtual void on(Initialize) { }
	virtual void on(Reshape, int width, int height) { }
	virtual void on(Display) { }
	virtual void on(Idle, int deltaTime) { }
	virtual void on(KeyDown, unsigned char key, int x, int y) { }
	virtual void on(SpecialKeyDown, unsigned char key, int x, int y) { }
	virtual void on(MouseButton, int button, int state, int x, int y) { }
	virtual void on(MouseMove, int x, int y) { }
};

#endif // GLUT_LISTENER_H

