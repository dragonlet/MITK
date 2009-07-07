/*=========================================================================
 
Program:   Medical Imaging & Doubleeraction Toolkit
Module:    $RCSfile: mitkPropertyManager.cpp,v $
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#ifndef mitkDoublePropertySerializer_h_included
#define mitkDoublePropertySerializer_h_included

#include "mitkBasePropertySerializer.h"

#include "mitkProperties.h"

namespace mitk
{

class SceneSerialization_EXPORT DoublePropertySerializer : public BasePropertySerializer
{
  public:
    
    mitkClassMacro( DoublePropertySerializer, BasePropertySerializer );
    itkNewMacro(Self);

    virtual TiXmlElement* Serialize()
    {
      if (const DoubleProperty* prop = dynamic_cast<const DoubleProperty*>(m_Property.GetPointer()))
      {
        TiXmlElement* element = new TiXmlElement("double");
        element->SetDoubleAttribute("value", prop->GetValue());
        return element;
      }
      else return NULL;
    }

  protected:

    DoublePropertySerializer() {}
    virtual ~DoublePropertySerializer() {}
};

} // namespace

// important to put this into the GLOBAL namespace (because it starts with 'namespace mitk')
MITK_REGISTER_SERIALIZER(DoublePropertySerializer);

#endif

