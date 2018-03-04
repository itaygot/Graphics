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
*/

#ifndef CAMERA_H
#define	CAMERA_H

#include "ogldev_math_3d.h"
#include "ogldev_keys.h"
//#include "ogldev_atb.h"

class Camera
{
public:

	Camera();

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

    bool OnKeyboard(OGLDEV_KEY Key);

    /*void OnMouse(int x, int y);*/

    //void OnRender();
	bool OnRender();

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

	/*void Rotate(const Quaternion& q);*/

	void ResetMousePos(int x, int y){
		m_mousePos.x = x;
		m_mousePos.y = y;
	}

	
	/*
	*	Originally created to enable resetting the 'm_mousePos' var. 
	*	Needed because after active mouse motion, after the release of the mouse, apparantly the 'passive' 
	*	mouse motion call-back is called, and if not resetted at the release mouse function, we get, 
	*	at the time of the 'passive'  mouse motion,  a non-zero deviation from the previous saved 
	*	location of the mouse - which causes an unintended view change of the camera.
	*/
	void OnMouseClick(int x, int y);


	void OnMouseMotion(int x, int y, bool active);

	/////////////

private:

    void Init();
    void Update();

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
};

#endif	/* CAMERA_H */

