project(org_mitk_example_gui_selectionservicemitk_views)

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE org_mitk_example_gui_selectionservicemitk_views_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDENCIES Qmitk
  NO_INSTALL
  )
