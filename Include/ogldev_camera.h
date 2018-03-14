/*

	Copyright 2010 Etay Meiri.		Edit: Itay Gothelf, 2018;

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

	Edit:
	- 'void OnRender()' --> 'bool OnRender()'.
	- Adding 'ResetMousePos(int, int)'.
	- Adding 'Camera::Rotate(Quaternion)'.

	2.11.18:
		- Commenting out 'ResetMousePos(int, int)'.
		- Commenting out 'Rotate(const Quaternion )'
		- Adding 'OnMouseClick(int, int)' - reset mouse pos' after clicks to allow drag motion, 
		- Adding 'OnMouseMoution(int, int, boolean)' instead of 'OnMouse(int, int): 
			Boolean parameter indicates if active (pressed) or passive mouse moution.

	3.4.18:
		- Adding back - Adding 'ResetMousePos(int, int)'.

	3.14.18:
		- Removing 'OnMouseClick(int, int)'. Camera implementation will take care of the
			resetting of the mouse position. 
		- Defining the 'MOUSE_ROTATION_METHOD' enum, and creating new member of that type
			dictating the type of mouse rotation the camera follows.
		- The method that checks if mouse is on the borders now is calld 'OnIdle()'
		- Adding bollean members 'mTargetChange' and 'mPosChange'
			Any method that changes an angle, should set on the 'mTargetChange' flag.
			any method that changes the position should set on the 'mPosChange' flag.
		- The method 'OnRender' should check if a target update is needed (and if so 
			call 'TargetUpdate()').
			It also should reset the 'mPosChange' flag.
*/

#ifndef CAMERA_H
#define	CAMERA_H

#include "ogldev_math_3d.h"
#include "ogldev_keys.h"
//#include "ogldev_atb.h"

class Camera
{
public:

	enum MOUSE_ROTATION_METHOD { ROTATION_DRAG, ROTATION_PASSIVE };

	MOUSE_ROTATION_METHOD m_MouseRotationMethod;
	

	Camera();

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

	void UpdateTarget();

	bool ShouldUpdate() {
		return mTargetChange | mPosChange;
	}

    void OnKeyboard(OGLDEV_KEY Key);

    //void OnRender();
	void OnRender();

    const Vector3f& GetPos() const
    {
        return m_pos;
    }

    const Vector3f& GetTarget() const
    {
        return m_target;
    }

    const Vector3f& GetUp() const
    {
        return m_up;
    }
    
    //void AddToATB(TwBar* bar);





	////////////
	void ResetMousePos(int x, int y){
		m_mousePos.x = x;
		m_mousePos.y = y;
	}

	
	//void OnMouseClick(int x, int y);

	void OnMouseMotion(int x, int y, bool active);

	void OnIdle();

	/////////////

private:

    void Init();
    

    Vector3f m_pos;
    Vector3f m_target;
    Vector3f m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    Vector2i m_mousePos;

	//////////////////////
	//bool m_shouldUpdate;
	bool mTargetChange;
	bool mPosChange;
	//////////////////////
};

#endif	/* CAMERA_H */

