#ifndef MITK_WIIMOTEHTREAD_H
#define MITK_WIIMOTEHTREAD_H

#include "mitkCommon.h"
#include "itkObject.h"

#include "itkMultiThreader.h"
#include "itkFastMutexLock.h"
#include "mitkCallbackFromGUIThread.h"

#include "wiimote.h"

#include "itksys/SystemTools.hxx" // used for GetTime()
#include "mitkVector.h"

namespace mitk
{
  // instead of include, enables this class to know the addon
  class WiiMoteAddOn;

  class WiiMoteThread : public itk::Object
  {
  public:

    // typedefs are used in context with CallbackFromGUIThread
    typedef itk::ReceptorMemberCommand<mitk::WiiMoteAddOn> ReceptorCommand;

    // eventually probs with Linux -> typename
    typedef ReceptorCommand::Pointer ReceptorCommandPointer;

    WiiMoteThread();
    ~WiiMoteThread();

    /**
    * Allows to set report types, detects extensions and responds to connect/disconnect <br>
    * events of extension, such as MotionPlus. 
    *
    * NOTE: don't access the public state from the 'remote' object here, as it will
    *	      be out-of-date (it's only updated via RefreshState() calls, and these
    *		    are reserved for the main application so it can be sure the values
    *		    stay consistent between calls).  Instead query 'new_state' only.
    *
    * @param remote 
    *          the old state of the connected Wii remote
    * @param changed
    *          the state change of the Wii remote
    * @param newState
    *          the new state, after the change is applied (i.e. new extension connected)
    *           
    */
    static void OnStateChange(wiimote &remote, state_change_flags  changed, const wiimote_state &newState);

    /**
    * Starts the thread for the Wiimote.
    */
    void Run();

    /**
    * Helper function, because the itk::MultiThreader can only <br>
    * start a new thread with a static member function.
    */
    static ITK_THREAD_RETURN_TYPE StartWiiMoteThread(void* data);

    /**
    * Connects the Wiimote and allows access to its functionality.
    */
    void StartWiiMote();

    /**
    * Stops the running thread.
    */
    void StopWiiMote();

    /**
    * Reconnects the Wiimote in case the connection is lost. 
    */
    void ReconnectWiiMote();

    /**
    * TODO
    * Detects all available Wiimotes. 
    */
    void DetectWiiMotes();

    /**
    * Reads incoming data from the IR camera, the acceleration sensors <br>
    * and the buttons. After processing the data (e.g. computations, assigning <br>
    * commands...) fires Wiimote events accordingly. 
    */
    void WiiMoteInput();

    void WiiMoteHomeButton();

  protected:

  private: 

    int m_ThreadID;
    itk::MultiThreader::Pointer m_MultiThreader;

    // mutex to control the flow of the method StartWiiMote()
    itk::FastMutexLock::Pointer m_WiiMoteThreadFinished;
    bool m_StopWiiMote;

    // access to the wiimote and parameter for callbackfromguithread
    wiimote m_WiiMote;
    ReceptorCommandPointer m_Command;

    // required for computation of movement
    Point2D m_LastReadData;
    double m_LastRecordTime;
    bool m_ReadDataOnce;

    // compensate delay
    // release: Skip = 1
    // debug: 
    int m_SkipTimeSteps;
    int m_CurrentTimeStep;

    //store all connected Wiimotes
    wiimote* m_WiiMotes[6];

  };
}

#endif // MITK_WIIMOTEHTREAD_H