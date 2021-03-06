/*

	Original file by:
		Copyright 2014 Etay Meiri

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Modified by:
		Itay Gothelf, 2017.
*/


#ifndef OGLDEV_CALLBACKS_H
#define OGLDEV_CALLBACKS_H

#include "ogldev_keys.h"
#include "ogldev_glew_backend.h"


class ICallbacks
{
public:

    virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState = OGLDEV_KEY_STATE_PRESS) {};
    
    //virtual void PassiveMouseCB(int x, int y) {};

	virtual void MousePassiveMotionCB(int x, int y) {};

    virtual void RenderSceneCB() {};

    virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y) {};

	virtual void IdleCB() {};

	virtual void TimerCB(int value) {};

	virtual void MouseActiveMotionCB(int x, int y) {};

	/*
	*	Default reshape behaviour, i.e. the same as glut's default
	*/
	virtual void ReshapeCB(int width, int height) {
		GLEWBackendViewport(0, 0, width, height);
	}
};


#endif