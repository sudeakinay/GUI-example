# GUI-example

## OssiloscopeProject
*Sude Akinay ve Yavuz Kiziltas tarafindan gelistirilmektedir.*

19 Haziran 2023'te baslanilan bu projede ilk olarak isterler belirlendi. Cross-compile özelligi ve gomulu sistemlerde daha cok kullanilmasi sebebiyle arayuz tasarim uygulamasi olarak LabView Yerine Qt Creator'in kullanilmasina karar verildi. Sifirdan Qt Creator ogrenilerek STM32 kartlari icin basit arayuz denemeleri yapildi. STM32'de ADC verisini UART hatti kullanarak alip verme islemi yapilarak hem mesaj paketi yapisi ogrenildi hem de UART haberlesmesi hakkinda bilgi birikimi olusturuldu. QT'de IMX8 icin cross-compile islemi ogrenildi ve bu baglamda Yocto SDK Kullanimi hakkinda dokuman olusturuldu. Projenin backend tarafi ayaga kaldirildigi icin son olarak arayuzden STM32 kartina genlik ve frekans verileri gonderildi. Bu verilere gore STM32 kartindan gelen voltaj degeri ekrana yazdirildi ve bununla birlikte projenin ilk asamasi ilk stajyer icin tamamlanmis oldu. 

Kullanilan kutuphaneler ve dokumanlari asagida siralanmistir:
| Library                         | Documentation|
| --------------------------------| ------------------------------------------------------------------------------ |
| Qt Configurations               | [Qt Creator Targets](https://doc.qt.io/qtcreator/creator-targets.html)         |
| QSerialPort                     | [QSerialPort Documentation](https://doc.qt.io/qt-5/qserialport.html)           |
| QCustomPlot                     | [QCustomPlot Introduction](https://www.qcustomplot.com/index.php/introduction) |
| Setup Yocto SDK with Qt support | [YOCTO SDK](https://mediawiki.compulab.com/w/index.php?title=Application_Notes:_Developing_with_Qt_on_CompuLab_platforms#Using_Yocto_SDK_with_QtCreator)                  |

Kullanilan uygulamalar:
- Qt Creator
- STM32F407 Discovery Kit
- STM32CubeIDE
- Termite
- Docklight
- Virtual Serial Ports Emulator
- [Notion](https://sude-akinay.notion.site/a69a1aeef97844f18feb622d34e78835?v=6975bd9fb4954894b86fe5a9addd142f&pvs=4)

  Bahsedilen ornek arayuz tasarimi asagidaki gibidir:
<p align="center">
  <img width="650" src="/images/image.jpg">
</p>



Yaz stajı bitiminden sonra projeye devam eden diğer stajyere iş aktarımları sağlanmıştır. Proje başında belirlediğimiz isterlere göre yeni eklemeler Yavuz Kızıltaş tarafından tamamlanamıştır. Osiloskop Arayüz Tasarımı'nın son hali aşağıdaki görsellerdeki gibidir.

<p align="center">
  <img width="650" src="/images/final-version-1.jpg">
</p>


<p align="center">
  <img width="650" src="/images/final-version-2.jpg">
</p>







