/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#ifndef MITK_APPLICATION_CURSOR_H_DEFINED_AND_ALL_IS_GOOD
#define MITK_APPLICATION_CURSOR_H_DEFINED_AND_ALL_IS_GOOD

namespace mitk
{

/*!
  \brief Toolkit specific implementation of mitk::ApplicationCursor

  For any toolkit, this class has to be sub-classed. One instance of that sub-class has to
  be registered with mitk::ApplicationCursor. See the (very simple) implmentation of 
  QmitkApplicationCursor for an example.
*/
class ApplicationCursorImplementation
{
  public:
    
    /// Change the current application cursor
    virtual void PushCursor(const char* XPM[], int hotspotX, int hotspotY) = 0;
    
    /// Restore the previous cursor
    virtual void PopCursor() = 0;

    virtual ~ApplicationCursorImplementation() {};

  protected:
  private:
};
  
/*!
  \brief Allows to override the application's cursor.

  Base class for classes that allow to override the applications cursor with context dependent 
  cursors. Accepts cursors in the XPM format.

  The behaviour is stack-like. You can push your cursor on top of the stack and later pop it to 
  reset the cursor to its former state. This is mimicking Qt's Application::setOverrideCuror() 
  behaviour, but should be ok for most cases where you want to switch a cursor.
*/
class ApplicationCursor
{
  public:

    /// This class is a singleton. 
    static ApplicationCursor* GetInstance();

    /// To be called by a toolkit specific ApplicationCursorImplementation. 
    void RegisterImplementation(ApplicationCursorImplementation* implementation);

    /// Change the current application cursor
    void PushCursor(const char* XPM[], int hotspotX = -1, int hotspotY = -1);
    
    /// Restore the previous cursor
    void PopCursor();
    
  protected:

    /// Purposely hidden - singleton
    ApplicationCursor();

  private:
    
    ApplicationCursorImplementation* m_Implementation;
    static ApplicationCursor* m_Instance;
};

} // namespace

#endif

