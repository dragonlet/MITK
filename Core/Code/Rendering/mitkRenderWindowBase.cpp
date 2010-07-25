/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-07-05 09:49:37 +0200 (Mo, 05 Jul 2010) $
Version:   $Revision: 24298 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "mitkRenderWindowBase.h"
#include "mitkDisplayPositionEvent.h"
#include "mitkVtkLayerController.h"
#include "mitkRenderingManager.h"
#include "vtkRenderer.h"


mitk::RenderWindowBase::RenderWindowBase( )
{

}

/*
* "Member functions, including virtual functions (10.3), can be called during construction or destruction (12.6.2). When a
* virtual function is called directly or indirectly from a constructor (including from the mem-initializer for a data member) or from
* a destructor, and the object to which the call applies is the object under construction or destruction, the function called is
* the one defined in the constructor or destructor�s own class or in one of its bases, but not a function overriding it in a class
* derived from the constructor or destructor�s class, or overriding it in one of the other base classes of the most derived object[..]"

* or short: within constructors and destructors classes are not polymorph.
*/
void mitk::RenderWindowBase::Initialize()
{
  mitk::RenderingManager*  renderingManager = mitk::RenderingManager::GetInstance();
  
  if(m_Renderer.IsNull())
  {
      m_Renderer = mitk::VtkPropRenderer::New( "asdf" , GetVtkRenderWindow(), renderingManager );
  }

  m_Renderer->InitRenderer(this->GetVtkRenderWindow());

  mitk::BaseRenderer::AddInstance(GetVtkRenderWindow(),m_Renderer);

  renderingManager->AddRenderWindow(GetVtkRenderWindow());

  m_RenderProp = vtkMitkRenderProp::New();
  m_RenderProp->SetPropRenderer(m_Renderer);
  m_Renderer->GetVtkRenderer()->AddViewProp(m_RenderProp);

  if((this->GetVtkRenderWindow()->GetSize()[0] > 10)
      && (this->GetVtkRenderWindow()->GetSize()[1] > 10))
    m_Renderer->InitSize(this->GetVtkRenderWindow()->GetSize()[0], this->GetVtkRenderWindow()->GetSize()[1]);

  m_InResize = false;  
}

void mitk::RenderWindowBase::Destroy()
{
  m_Renderer->GetRenderingManager()->RemoveRenderWindow(GetVtkRenderWindow());
  mitk::BaseRenderer::RemoveInstance(GetVtkRenderWindow());
  m_Renderer->GetVtkRenderer()->RemoveViewProp(m_RenderProp);
  m_RenderProp->Delete();
}

mitk::RenderWindowBase::~RenderWindowBase()
{

}

void mitk::RenderWindowBase::mousePressMitkEvent(mitk::MouseEvent *me)
{
  if (m_Renderer.IsNotNull())
    m_Renderer->MousePressEvent(me);
}

void mitk::RenderWindowBase::mouseReleaseMitkEvent(mitk::MouseEvent *me)
{
  if(m_Renderer.IsNotNull())
    m_Renderer->MouseReleaseEvent(me);
}

void mitk::RenderWindowBase::mouseMoveMitkEvent(mitk::MouseEvent *me)
{
  if (m_Renderer.IsNotNull()) 
     m_Renderer->MouseMoveEvent(me);
}

void mitk::RenderWindowBase::wheelMitkEvent(mitk::WheelEvent *we)
{
  if ( !m_ProcessWheelEvents )
    return;
 
  if ( !GetSliceNavigationController()->GetSliceLocked() )
  {
    mitk::Stepper* stepper = GetSliceNavigationController()->GetSlice();

    if (stepper->GetSteps() <= 1)
    {
      stepper = GetSliceNavigationController()->GetTime();
    }

    //if (we->orientation() * we->GetDelta()  > 0)
    if (we->GetDelta()  > 0)
    {
      stepper->Next();
    }
    else
    {
      stepper->Previous();
    }

    //also send to Renderer to send if to MITK interaction mechanism
    if(m_Renderer.IsNotNull()) 
      m_Renderer->WheelEvent(we);
    
  }
}

void mitk::RenderWindowBase::keyPressMitkEvent(mitk::KeyEvent* mke)
{
  if (m_Renderer.IsNotNull())
    m_Renderer->KeyPressEvent(mke);
}


void mitk::RenderWindowBase::resizeMitkEvent(int width, int height)
{
  if(m_InResize) //@FIXME CRITICAL probably related to VtkSizeBug
    return;
  m_InResize = true;

  if(m_Renderer.IsNotNull())
  {
    m_Renderer->Resize(width, height);
  }

  m_InResize = false;
}


mitk::SliceNavigationController * mitk::RenderWindowBase::GetSliceNavigationController()
{
  return mitk::BaseRenderer::GetInstance(this->GetVtkRenderWindow())->GetSliceNavigationController();
}

mitk::CameraRotationController * mitk::RenderWindowBase::GetCameraRotationController()
{
  return mitk::BaseRenderer::GetInstance(this->GetVtkRenderWindow())->GetCameraRotationController();
}

mitk::BaseController * mitk::RenderWindowBase::GetController()
{
  mitk::BaseRenderer * renderer = mitk::BaseRenderer::GetInstance(GetVtkRenderWindow());
  switch ( renderer->GetMapperID() )
  {
    case mitk::BaseRenderer::Standard2D:
      return GetSliceNavigationController();

    case mitk::BaseRenderer::Standard3D:
      return GetCameraRotationController();

    default:
      return GetSliceNavigationController();
  }
}

mitk::VtkPropRenderer* mitk::RenderWindowBase::GetRenderer()
{
  return m_Renderer;
}



void mitk::RenderWindowBase::SetProcessWheelEvents( bool state )
{
  m_ProcessWheelEvents = state;
}

bool mitk::RenderWindowBase::GetProcessWheelEvents()
{
  return m_ProcessWheelEvents;
}
