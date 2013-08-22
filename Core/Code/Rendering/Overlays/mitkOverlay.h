/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef OVERLAY_H
#define OVERLAY_H

#include <MitkExports.h>
#include <mitkCommon.h>
#include <mitkBaseRenderer.h>

namespace mitk {

class BaseLayouter;

/** \brief Base class for all overlays */
/** This class is to be implemented in order to create overlays which are managed by the OverlayManager and can be placed by a BaseLayouter.
This class contains an internal Propertylist, and another PropertyList for each BaseRenderer.
A property that is specified for a specific renderer always overrides the general internal property of the same name.
AddOverlay, RemoveOverlay and UpdateOverlay methods have to be implemented.*/
class MITK_CORE_EXPORT Overlay : public itk::Object {
  friend class BaseLayouter;
public:

  /** \brief Container for position and size on the display.*/
  struct Bounds
  {
    itk::Point<double,2> Position;
    itk::Point<double,2> Size;
  };

  typedef std::map<const BaseRenderer*,PropertyList::Pointer> MapOfPropertyLists;

  /** \brief Base class for mapper specific rendering ressources.
   */
  class MITK_CORE_EXPORT BaseLocalStorage
  {
  public:


    bool IsGenerateDataRequired(mitk::BaseRenderer *renderer,mitk::Overlay *overlay);

    inline void UpdateGenerateDataTime()
    {
      m_LastGenerateDataTime.Modified();
    }

    PropertyList::Pointer GetPropertyList();

    inline itk::TimeStamp & GetLastGenerateDataTime() { return m_LastGenerateDataTime; }



  protected:

    /** \brief timestamp of last update of stored data */
    itk::TimeStamp m_LastGenerateDataTime;

  };

  //##Documentation
  //## @brief Set the property (instance of BaseProperty) with key @a propertyKey in the PropertyList
  //## of the @a renderer (if NULL, use BaseRenderer-independent PropertyList). This is set-by-value.
  //##
  //## @warning Change in semantics since Aug 25th 2006. Check your usage of this method if you do
  //##          more with properties than just call <tt>SetProperty( "key", new SomeProperty("value") )</tt>.
  //##
  //## @sa GetProperty
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  void SetProperty(const char *propertyKey, BaseProperty* property, const mitk::BaseRenderer* renderer = NULL);

  //##Documentation
  //## @brief Replace the property (instance of BaseProperty) with key @a propertyKey in the PropertyList
  //## of the @a renderer (if NULL, use BaseRenderer-independent PropertyList). This is set-by-reference.
  //##
  //## If @a renderer is @a NULL the property is set in the BaseRenderer-independent
  //## PropertyList of this Overlay.
  //## @sa GetProperty
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  void ReplaceProperty(const char *propertyKey, BaseProperty* property, const mitk::BaseRenderer* renderer = NULL);

  //##Documentation
  //## @brief Add the property (instance of BaseProperty) if it does
  //## not exist (or always if \a overwrite is \a true)
  //## with key @a propertyKey in the PropertyList
  //## of the @a renderer (if NULL, use BaseRenderer-independent
  //## PropertyList). This is set-by-value.
  //##
  //## For \a overwrite == \a false the property is \em not changed
  //## if it already exists. For \a overwrite == \a true the method
  //## is identical to SetProperty.
  //##
  //## @sa SetProperty
  //## @sa GetProperty
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  void AddProperty(const char *propertyKey, BaseProperty* property, const mitk::BaseRenderer* renderer = NULL, bool overwrite = false);

  //##Documentation
  //## @brief Get the PropertyList of the @a renderer. If @a renderer is @a
  //## NULL, the BaseRenderer-independent PropertyList of this Overlay
  //## is returned.
  //## @sa GetProperty
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  mitk::PropertyList* GetPropertyList(const mitk::BaseRenderer* renderer = NULL) const;

  //##Documentation
  //## @brief Add values from another PropertyList.
  //##
  //## Overwrites values in m_PropertyList only when possible (i.e. when types are compatible).
  //## If you want to allow for object type changes (replacing a "visible":BoolProperty with "visible":IntProperty,
  //## set the @param replace.
  //##
  //## @param replace true: if @param pList contains a property "visible" of type ColorProperty and our m_PropertyList also has a "visible" property of a different type (e.g. BoolProperty), change the type, i.e. replace the objects behind the pointer.
  //##
  //## @sa SetProperty
  //## @sa ReplaceProperty
  //## @sa m_PropertyList
  void ConcatenatePropertyList(PropertyList* pList, bool replace = false);

  //##Documentation
  //## @brief Get the property (instance of BaseProperty) with key @a propertyKey from the PropertyList
  //## of the @a renderer, if available there, otherwise use the BaseRenderer-independent PropertyList.
  //##
  //## If @a renderer is @a NULL or the @a propertyKey cannot be found
  //## in the PropertyList specific to @a renderer or is disabled there, the BaseRenderer-independent
  //## PropertyList of this Overlay is queried.
  //## @sa GetPropertyList
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  mitk::BaseProperty* GetProperty(const char *propertyKey, const mitk::BaseRenderer* renderer = NULL) const;

  //##Documentation
  //## @brief Get the property of type T with key @a propertyKey from the PropertyList
  //## of the @a renderer, if available there, otherwise use the BaseRenderer-independent PropertyList.
  //##
  //## If @a renderer is @a NULL or the @a propertyKey cannot be found
  //## in the PropertyList specific to @a renderer or is disabled there, the BaseRenderer-independent
  //## PropertyList of this Overlay is queried.
  //## @sa GetPropertyList
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  template <typename T>
    bool GetProperty(itk::SmartPointer<T> &property, const char *propertyKey, const mitk::BaseRenderer* renderer = NULL) const
  {
    property = dynamic_cast<T *>(GetProperty(propertyKey, renderer));
    return property.IsNotNull();
  }

  //##Documentation
  //## @brief Get the property of type T with key @a propertyKey from the PropertyList
  //## of the @a renderer, if available there, otherwise use the BaseRenderer-independent PropertyList.
  //##
  //## If @a renderer is @a NULL or the @a propertyKey cannot be found
  //## in the PropertyList specific to @a renderer or is disabled there, the BaseRenderer-independent
  //## PropertyList of this Overlay is queried.
  //## @sa GetPropertyList
  //## @sa m_PropertyList
  //## @sa m_MapOfPropertyLists
  template <typename T>
    bool GetProperty(T* &property, const char *propertyKey, const mitk::BaseRenderer* renderer = NULL) const
  {
    property = dynamic_cast<T *>(GetProperty(propertyKey, renderer));
    return property!=NULL;
  }

  //##Documentation
  //## @brief Convenience access method for GenericProperty<T> properties
  //## (T being the type of the second parameter)
  //## @return @a true property was found
  template <typename T>
    bool GetPropertyValue(const char* propertyKey, T & value, mitk::BaseRenderer* renderer=NULL) const
    {
      GenericProperty<T>* gp= dynamic_cast<GenericProperty<T>*>(GetProperty(propertyKey, renderer));
      if ( gp != NULL )
      {
        value = gp->GetValue();
        return true;
      }
      return false;
    }

  //##Documentation
  //## @brief Convenience access method for bool properties (instances of
  //## BoolProperty)
  //## @return @a true property was found
  bool GetBoolProperty(const char* propertyKey, bool &boolValue, mitk::BaseRenderer* renderer = NULL) const;

  //##Documentation
  //## @brief Convenience access method for int properties (instances of
  //## IntProperty)
  //## @return @a true property was found
  bool GetIntProperty(const char* propertyKey, int &intValue, mitk::BaseRenderer* renderer=NULL) const;

  //##Documentation
  //## @brief Convenience access method for float properties (instances of
  //## FloatProperty)
  //## @return @a true property was found
  bool GetFloatProperty(const char* propertyKey, float &floatValue, mitk::BaseRenderer* renderer = NULL) const;

  //##Documentation
  //## @brief Convenience access method for string properties (instances of
  //## StringProperty)
  //## @return @a true property was found
  bool GetStringProperty(const char* propertyKey, std::string& string, mitk::BaseRenderer* renderer = NULL) const;

  //##Documentation
  //## @brief Convenience method for setting int properties (instances of
  //## IntProperty)
  void SetIntProperty(const char* propertyKey, int intValue, mitk::BaseRenderer* renderer=NULL);

  //##Documentation
  //## @brief Convenience method for setting int properties (instances of
  //## IntProperty)
  void SetBoolProperty(const char* propertyKey, bool boolValue, mitk::BaseRenderer* renderer=NULL);

  //##Documentation
  //## @brief Convenience method for setting int properties (instances of
  //## IntProperty)
  void SetFloatProperty(const char* propertyKey, float floatValue, mitk::BaseRenderer* renderer=NULL);

  //##Documentation
  //## @brief Convenience method for setting int properties (instances of
  //## IntProperty)
  void SetStringProperty(const char* propertyKey, const char* string, mitk::BaseRenderer* renderer=NULL);

  //##Documentation
  //## @brief Convenience access method for boolean properties (instances
  //## of BoolProperty). Return value is the value of the property. If the property is
  //## not found, the value of @a defaultIsOn is returned.
  //##
  //## Thus, the return value has a different meaning than in the
  //## GetBoolProperty method!
  //## @sa GetBoolProperty
  bool IsOn(const char* propertyKey, mitk::BaseRenderer* renderer, bool defaultIsOn = true) const
  {
    if(propertyKey==NULL)
      return defaultIsOn;
    GetBoolProperty(propertyKey, defaultIsOn, renderer);
    return defaultIsOn;
  }

  //##Documentation
  //## @brief Convenience access method for accessing the name of an object (instance of
  //## StringProperty with property-key "name")
  //## @return @a true property was found
  bool GetName(std::string& nodeName, mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "name") const
  {
    return GetStringProperty(propertyKey, nodeName, renderer);
  }

  //##Documentation
  //## @brief Extra convenience access method for accessing the name of an object (instance of
  //## StringProperty with property-key "name").
  //##
  //## This method does not take the renderer specific
  //## propertylists into account, because the name of an object should never be renderer specific.
  //## @returns a std::string with the name of the object (content of "name" Property).
  //## If there is no "name" Property, an empty string will be returned.
  virtual std::string GetName() const
  {
    mitk::StringProperty* sp = dynamic_cast<mitk::StringProperty*>(this->GetProperty("name"));
    if (sp == NULL)
      return "";
    return sp->GetValue();
  }

  //##Documentation
  //## @brief Extra convenience access method to set the name of an object.
  //##
  //## The name will be stored in the non-renderer-specific PropertyList in a StringProperty named "name".
  virtual void SetName( const char* name)
  {
    if (name == NULL)
      return;
    this->SetProperty("name", StringProperty::New(name));
  }

  //##Documentation
  //## @brief Extra convenience access method to set the name of an object.
  //##
  //## The name will be stored in the non-renderer-specific PropertyList in a StringProperty named "name".
  virtual void SetName( const std::string name)
  {
    this->SetName(name.c_str());
  }

  //##Documentation
  //## @brief Convenience access method for color properties (instances of
  //## ColorProperty)
  //## @return @a true property was found
  bool GetColor(float rgb[], mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "color") const;
  //##Documentation
  //## @brief Convenience method for setting color properties (instances of
  //## ColorProperty)
  void SetColor(const mitk::Color &color, mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "color");
  //##Documentation
  //## @brief Convenience method for setting color properties (instances of
  //## ColorProperty)
  void SetColor(float red, float green, float blue, mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "color");
  //##Documentation
  //## @brief Convenience method for setting color properties (instances of
  //## ColorProperty)
  void SetColor(const float rgb[], mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "color");
  //##Documentation
  //## @brief Convenience access method for opacity properties (instances of
  //## FloatProperty)
  //## @return @a true property was found
  bool GetOpacity(float &opacity, mitk::BaseRenderer* renderer, const char* propertyKey = "opacity") const;
  //##Documentation
  //## @brief Convenience method for setting opacity properties (instances of
  //## FloatProperty)
  void SetOpacity(float opacity, mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "opacity");

  void SetText(std::string text);

  std::string GetText();

  void SetFontSize(int fontSize);

  int GetFontSize();

  //##Documentation
  //## @brief Convenience access method for visibility properties (instances
  //## of BoolProperty with property-key "visible")
  //## @return @a true property was found
  //## @sa IsVisible
  bool GetVisibility(bool &visible, mitk::BaseRenderer* renderer, const char* propertyKey = "visible") const
  {
    return GetBoolProperty(propertyKey, visible, renderer);
  }

  //##Documentation
  //## @brief Convenience access method for visibility properties (instances
  //## of BoolProperty). Return value is the visibility. Default is
  //## visible==true, i.e., true is returned even if the property (@a
  //## propertyKey) is not found.
  //##
  //## Thus, the return value has a different meaning than in the
  //## GetVisibility method!
  //## @sa GetVisibility
  //## @sa IsOn
  bool IsVisible(mitk::BaseRenderer* renderer, const char* propertyKey = "visible", bool defaultIsOn = true) const
  {
    return IsOn(propertyKey, renderer, defaultIsOn);
  }

  //##Documentation
  //## @brief Convenience method for setting visibility properties (instances
  //## of BoolProperty)
  //## @param visible If set to true, the data will be rendered. If false, the render will skip this data.
  //## @param renderer Specify a renderer if the visibility shall be specific to a renderer
  //## @param propertykey Can be used to specify a user defined name of the visibility propery.
  void SetVisibility(bool visible, mitk::BaseRenderer* renderer = NULL, const char* propertyKey = "visible");

  /** \brief Adds the overlay to the specified renderer. Update Overlay should be called soon in order to apply all properties*/
  virtual void AddOverlay(BaseRenderer *renderer) = 0;

  /** \brief Removes the overlay from the specified renderer. It is not visible anymore then.*/
  virtual void RemoveOverlay(BaseRenderer *renderer) = 0;

  /** \brief Applies all properties and should be called before the rendering procedure.*/
  virtual void UpdateOverlay(BaseRenderer *renderer) = 0;

  /** \brief Returns position and size of the overlay on the display.*/
  virtual Bounds GetBoundsOnDisplay(BaseRenderer *renderer) = 0;

  /** \brief Sets position and size of the overlay on the display.*/
  virtual void SetBoundsOnDisplay(BaseRenderer *renderer, Bounds) = 0;

  mitkClassMacro(Overlay, itk::Object);

protected:

  /** \brief explicit constructor which disallows implicit conversions */
  explicit Overlay();

  /** \brief virtual destructor in order to derive from this class */
  virtual ~Overlay();

  //##Documentation
  //## @brief BaseRenderer-independent PropertyList
  //##
  //## Properties herein can be overwritten specifically for each BaseRenderer
  //## by the BaseRenderer-specific properties defined in m_MapOfPropertyLists.
  PropertyList::Pointer m_PropertyList;

  //##Documentation
  //## @brief Map associating each BaseRenderer with its own PropertyList
  mutable MapOfPropertyLists m_MapOfPropertyLists;

  //##Documentation
  //## @brief Timestamp of the last change of m_Data
  itk::TimeStamp m_DataReferenceChangedTime;

  unsigned long m_PropertyListModifiedObserverTag;

private:

  /** \brief copy constructor */
  Overlay( const Overlay &);

  /** \brief assignment operator */
  Overlay &operator=(const Overlay &);

  /** \brief Reference to the layouter in which this overlay is managed. */
  BaseLayouter* m_LayoutedBy;

};

} // namespace mitk
#endif // OVERLAY_H


