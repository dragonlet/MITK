#include "mitkImage.h"
#include "mitkDataTreeNodeFactory.h"
#include "mitkCylindricToCartesianFilter.h"
#include "mitkImageSliceSelector.h"

#include <fstream>
int mitkCylindricToCartesianFilterTest(int argc, char* argv[])
{
  std::cout << "Loading file: ";
  if(argc==0)
  {
    std::cout<<"no file specified [FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  mitk::Image::Pointer image = NULL;
  mitk::DataTreeNodeFactory::Pointer factory = mitk::DataTreeNodeFactory::New();
  try
  {
    factory->SetFileName( argv[1] );
    factory->Update();

    if(factory->GetNumberOfOutputs()<1)
    {
      std::cout<<"file could not be loaded [FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    mitk::DataTreeNode::Pointer node = factory->GetOutput( 0 );
    image = dynamic_cast<mitk::Image*>(node->GetData());
    if(image.IsNull())
    {
      std::cout<<"file not an image - test will not be applied [PASSED]"<<std::endl;
      std::cout<<"[TEST DONE]"<<std::endl;
      return EXIT_SUCCESS;
    }
  }
  catch ( itk::ExceptionObject & ex )
  {
    std::cout << "Exception: " << ex << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }

  //Use CylindricToCartesianFilter
	mitk::CylindricToCartesianFilter::Pointer cyl2cart = mitk::CylindricToCartesianFilter::New();
    cyl2cart->SetInput(image);
    cyl2cart->SetTargetXSize( 64 );

  //Take a slice
	mitk::ImageSliceSelector::Pointer slice = mitk::ImageSliceSelector::New();
	  slice->SetInput(cyl2cart->GetOutput());
	  slice->SetSliceNr(1);
	  slice->Update();

  std::cout << "Testing IsInitialized(): ";
  if(slice->GetOutput()->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsSliceSet(): ";
  if(slice->GetOutput()->IsSliceSet(0)==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  if(image->GetDimension(3) > 1)
  {
    int time=image->GetDimension(3)-1;

    std::cout << "Testing 3D+t: Setting time to " << time << ": ";
	  slice->SetTimeNr(time);
    if(slice->GetTimeNr()!=time)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: Updating slice: ";
    slice->Update();
    if(slice->GetOutput()->IsInitialized()==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: IsSliceSet(): ";
    if(slice->GetOutput()->IsSliceSet(0)==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;

    std::cout << "Testing 3D+t: First slice in reader available: ";
    if(image->IsSliceSet(0, time)==false)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
    std::cout<<"[PASSED]"<<std::endl;
  }

  std::cout<<"[TEST DONE]"<<std::endl;
  return EXIT_SUCCESS;
}
