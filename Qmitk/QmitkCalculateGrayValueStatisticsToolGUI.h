/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile: mitk.cpp,v $
Language:  C++
Date:      $Date$
Version:   $Revision: 1.0 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QmitkCalculateGrayValueStatisticsToolGUI_h_Included
#define QmitkCalculateGrayValueStatisticsToolGUI_h_Included

#include "QmitkToolGUI.h"

#include "mitkCalculateGrayValueStatisticsTool.h"

/**
 \brief GUI for mitk::CalculateGrayValueStatisticsTool.

 Shows nothing. Only when the corresponding tool send a message that statistics are ready, this class pops up a window showing the results.

 Last contributor: $Author$
*/
class QMITK_EXPORT QmitkCalculateGrayValueStatisticsToolGUI : public QmitkToolGUI
{
  Q_OBJECT

  public:

    mitkClassMacro(QmitkCalculateGrayValueStatisticsToolGUI, QmitkToolGUI);
    itkNewMacro(QmitkCalculateGrayValueStatisticsToolGUI);

  virtual ~QmitkCalculateGrayValueStatisticsToolGUI();

  /**
   - reacts to signals from mitk::CalculateGrayValueStatisticsTool
   - shows the histogram and report widget
   */
  void OnCalculationsDone();

  signals:

  public slots:

  protected slots:

    /// Connected to signal from QmitkToolGUI. We remember the current tool here
    void OnNewToolAssociated(mitk::Tool*);

  protected:

    QmitkCalculateGrayValueStatisticsToolGUI();

    mitk::CalculateGrayValueStatisticsTool::Pointer m_CalculateGrayValueStatisticsTool;
};

#endif

