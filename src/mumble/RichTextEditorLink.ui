<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>RichTextEditorLink</class>
 <widget class="QDialog" name="RichTextEditorLink">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>295</width>
    <height>95</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>Add Link</string>
  </property>
  <layout class="QGridLayout" name="gridLayout">
   <item row="0" column="0">
    <widget class="QLabel" name="qlUrl">
     <property name="text">
      <string>URL</string>
     </property>
     <property name="buddy">
      <cstring>qleUrl</cstring>
     </property>
    </widget>
   </item>
   <item row="0" column="1">
    <widget class="QLineEdit" name="qleUrl"/>
   </item>
   <item row="1" column="0">
    <widget class="QLabel" name="qlText">
     <property name="text">
      <string>Text</string>
     </property>
     <property name="buddy">
      <cstring>qleText</cstring>
     </property>
    </widget>
   </item>
   <item row="1" column="1">
    <widget class="QLineEdit" name="qleText"/>
   </item>
   <item row="2" column="0" colspan="2">
    <widget class="QDialogButtonBox" name="buttonBox">
     <property name="orientation">
      <enum>Qt::Horizontal</enum>
     </property>
     <property name="standardButtons">
      <set>QDialogButtonBox::Cancel|QDialogButtonBox::Ok</set>
     </property>
    </widget>
   </item>
  </layout>
 </widget>
 <tabstops>
  <tabstop>qleUrl</tabstop>
  <tabstop>qleText</tabstop>
  <tabstop>buttonBox</tabstop>
 </tabstops>
 <resources/>
 <connections>
  <connection>
   <sender>buttonBox</sender>
   <signal>accepted()</signal>
   <receiver>RichTextEditorLink</receiver>
   <slot>accept()</slot>
   <hints>
    <hint type="sourcelabel">
     <x>227</x>
     <y>228</y>
    </hint>
    <hint type="destinationlabel">
     <x>157</x>
     <y>274</y>
    </hint>
   </hints>
  </connection>
  <connection>
   <sender>buttonBox</sender>
   <signal>rejected()</signal>
   <receiver>RichTextEditorLink</receiver>
   <slot>reject()</slot>
   <hints>
    <hint type="sourcelabel">
     <x>295</x>
     <y>234</y>
    </hint>
    <hint type="destinationlabel">
     <x>286</x>
     <y>274</y>
    </hint>
   </hints>
  </connection>
 </connections>
</ui>
