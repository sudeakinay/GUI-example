# QT CREATOR 

Kullanilan kutuphaneler ve dokumanlari asagida siralanmistir:
|            Kutuphane            |          Amac             |                           Dokuman                                              |
| --------------------------------|---------------------------| ------------------------------------------------------------------------------ |
| Qt Configurations               | Derleme icin kit ayarlari | [Qt Creator Configurations](https://doc.qt.io/qtcreator/creator-targets.html)  |
| QSerialPort                     | UART Haberlesmesi icin    | [QSerialPort Documentation](https://doc.qt.io/qt-5/qserialport.html)           |
| QCustomPlot                     | Grafik olusturmak icin    | [QCustomPlot Introduction](https://www.qcustomplot.com/index.php/introduction) |
| Setup Yocto SDK with Qt support | Cross-compiling icin      | [YOCTO SDK](https://mediawiki.compulab.com/w/index.php?title=Application_Notes:_Developing_with_Qt_on_CompuLab_platforms#Using_Yocto_SDK_with_QtCreator)                                              |

---
## Denemeler
Ilk olarak asagidaki gibi basit bir progress bar uygulamasi gerceklestirildi. Bu basit uygulama temel alinarak STM32'de gerekli gomulu yazilim olusturulduktan sonra potansiyometreden alinan ADC verisi UART hattina basildi ve arayuzde bu veriye gore progress barin saga sola kaymasi gozlemlendi.

<p align="center">
  <img width="350" src="/images/progress-bar.jpg">
</p>

Daha sonra farkli sinyal tipleri olusturan slotlar tanimlanarak olusturulan bu fake datalar UART hattindan aliniyormus gibi arayuze aktarildi. Genlik ve Frekans butonlari tanımlanarak bu butonlara gore sinyaldeki degisimler kontrol edildi. Asagida bir fake sinus dalgasi olusturan arayuz tasarimi ornegi gosterilmistir.

<p align="center">
  <img width="475" src="/images/osiloskop-2-25.jpg">
    <img width="475" src="/images/osiloskop-5-50.jpg">
</p>

Bu basit tasarimdan sonra son olarak bu repoda bulunan projenin son hali olusturuldu. Seri porttan zaman ve voltaj verisi alarak bu verileri x-y axislerine basan arayuz tasarimi gerceklestirildi. Ayni zamanda kullanicidan butonlar yardimiyle aldigi frekans ve genlik degerini "frekans_verisi, ',', 'genlik_verisi', '\n" şeklinde paket yapisi olarak UART hattina basarak STM32 kartina gonderilir. Kullanicinin girdigi bu degerlere gore zaman ve voltaj degeri veri akisi saglanmis olur. Sonucta ekranda gozlemlenen arayuz asagidaki gibidir:

<p align="center">
  <img width="475" src="/images/osiloskop-son-v1.jpg">
    <img width="475" src="/images/osiloskop-son-v2.jpg">
</p>


----
## Cross - Compiling

Yocto SDK paketinin [buradaki](https://drive.google.com/file/d/14nEqaVNputvIwf5rw9Hv_kAxjfaiFbpX/view) linkten indirilmesi gerekiyor.
1. Diskte en az 12GB varsa linkteki Qt5 support image indirilir.
2. İcinde yeterince yer bulunan bir dosya yolu secilerek burasi asagidaki komutla PATH_TO_SDK olarak set edilir.

         export PATH_TO_SDK = /path/to/sdk

3. indirilen SDK dosyasi secilen bu dosya yolunda asagidaki komutla extract edilir. Bu asama, indirilen dosya cok buyuk oldugu icin uzun surecektir.

         sudo tar -C ${PATH_TO_SDK} -xf /path/to/qt5-build-env.tar.bz2

4. Asagidaki komutlar calistirilir

         sudo mkdir -p /opt/fsl-imx-xwayland/4.14-sumo
         sudo mount -B ${PATH_TO_SDK}/opt/fsl-imx-xwayland/4.14-sumo /opt/fsl-imx-xwayland/4.14-sumo


5. Son olarak asagidaki adim kullanilarak environment ortamini set etme scripti aktiflestirilir.

         . /opt/fsl-imx-swayland/4.14-sumo/environment-setup-aarch64-poky-linux

      - Bu adimlarin yeni bir terminalde SDK kullanilacagi zaman bastan yapilmasi gerekir.
      - Buraya kadar her sey dogru gittiyse muhtemelen unset library hatasi alinacak. Asagidaki komutu calistirdiktan sonra 5. adimda verilen komutu calistirmayi tekrar deneyin.

       unset LD_LIBRARY_PATH


Yocto SDK basariyla indirilip kurulduktan sonra Linux makinesinde bunu kullanmak icin gerekli konfigurasyonlar asagidaki gibi yapılır:
1. Build-essentials indirilir.

         sudo apt-get install build-essential

2. Qt Creator indirilir.

         sudo apt-get install qtcreator

3. Yocto SDK kurulumu kontrol edilir

         [[ -f /opt/fsl-imx-xwayland/4.14-sumo/environment-setup-aarch64-poky-linux ]] && echo “Install” || echo “Not Installed “

4. Qt5 support kontrol edilir.

         [[ -f /opt/fsl-imx-xwayland/4.14-sumo/sysroots/x86_64-pokysdk-linux/usr/bin/qt5/qmake ]] && echo “enabled” || echo “off “


### Qt Creator Olmadan Yocto SDK Kullanimi
1. Yeni bir terminal acilir ve cross compiling ortamı asagidaki komutla set edilir.

         . /opt/fsl-imx-xwayland/4.14-sumo/environment-setup-aarch64-poky-linux

2. Ornek Qt5 projelerinin yer aldigi klasör ve icindeki tum dosyalarin komutu calistiran kullaniciya ve kullanicinin ait oldugu gruplara atama islemi asagidaki komutla gerceklestirilir.

         sudo chown ${USER}::${GROUPS} -R /opt/fsl-imx-xwayland/4.14-sumo/sysroots/aarch64-poky-linux/usr/share/qt5/examples

3. Yocto SDK indirildiginde ornek olarak verilen projelerden biri olan opengl/hellogl2 projesi acilir

         cd /opt/fsl-imx-xwayland/4.14-sumo/sysroots/aarch64-poky-linux/usr/share/qt5/examples/opengl/hellogl2

4. Sirayla asagidaki komutlar gerceklestirilir.

         qmake -config release
         make -j 8

6. Olusturulan binary dosyanın ARM64 ile executable olup olmadigi asagidaki komut ile kontrol edilir

         file hellogl2 | grep -q aarch64 && echo “good” || echo “bad”

 
Bu komutun sonucu olarak eger good gelirse olusturulan dosya dogrudur. Hedef cihaza aktarilarak kosturulabilir. Eger sonuc bad olarak gelirse az once gerceklestirilen make komutu clean edildiktn sonra tum process tekrar edilir.

### Qt Creator ile Yocto SDK Kullanimi
1. Yeni bir terminal acilir ve cross compiling ortami asagidaki komutla set edilir.

         . /opt/fsl-imx-xwayland/4.14-sumo/environment-setup-aarch64-poky-linux

2. Ayni terminalde Qt Creator asagidaki komutla acilir.

         Qtcreator

3. Qt Creator uygulamasi acildiktan sonra Tools --> Options kısmından Kits secenegi secilir.
   
4. Kits ekraninda add manual olarak secilerek yeni olusturulan kite X-compile-ARM-64bit gibi bir isim verilir. Qt version secilirken qmake yolu olarak asagidaki yol girilir. Apply tusuna basilarak yeni olusturulan kite bu qt versiyonu kaydedilir.

         /opt/fsl-imx-xwayland/4.14-sumo/sysroots/x86_64-pokysdk-linux/usr/bin/qt5/qmake

5. C compileri olarak asagidaki yol girilir ve Compilers tab olarak ABI arm-linux-generic-elf-64bit secilir

         opt/fsl-imx-xwayland/4.14-sumo/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gcc

6. C++ compileri olarak asagidaki yol girilir ve Compilers tab olarak ABI arm-linux-generic-elf-64bit secilir

         /opt/fsl-imx-xwayland/4.14-sumo/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++

7. Debugger yolu olarak asagidaki yol girilir.
 
         /opt/fsl-imx-xwayland/4.14-sumo/sysroots/x86_64-pokysdk-linux/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gdb

8. Devices seceneginde manage tusuna basilarak yeni bir device eklenir. Generic Linux Device adinda yeni bir linux makinesi eklenir. IP adresi gibi cesitli ozellikler girilerek bu kisim ozellestirilir. Asagida bir ornegi verilmistir.
   
<p align="center">
  <img width="450" src="/images/device.png">
</p>

9. Kits kismina donulerek device type olarak olusturulan bir generic linux device secilir.

10. Sysroot yolu olarak asagidaki yol girilir.

         /opt/fsl-imx-xwayland/4.14-sumo/sysroots/aarch64-poky-linux

11. Apply tusuna basilarak tum ozellikler kaydedilerek Qt Creatordan cikilir.

En sonda olusan kit ayarlari asagidaki gibidir.
<p align="center">
  <img width="450" src="/images/kits.png">
</p>








