# Preprocessing4Semantic
**Linux-cpp-python** repository : Images Preprocessing for Semantic Segmentation

### 1. Library Dependency

at least: **opencv 2.4.3**  or **opencv 3.0.x**

python : opencv-python, labelme(option) ...

### 2. Preparatory Work

1. **Dataset Struct:** example for KITTI

    data_semantics
    ├── testing
    │   └── image_2

    └── training
        ├── image_2

    ​	├── semantic

    Folder named image_2 is Image sequence for RGB with the suffix ".png" ;  Folder named semantic is  Image sequence for annotation of semantic segmentation. And in this folder, each Image is uint8 8bit in single channel,  with the suffix ".png", Each pixel value in image represents a semantic category ID. Attention : name of the same image in "image_2" folder or "semantic" folder is also the same.

2. **Modification of documents :**

    + ./src/Preprocessing.cc: replace "strPatternRGB" and "strPatternSem" with your own folder's name.
    + ./app/main.cpp: adjust percentage values for each category for image preprocessing.

### 3. How to Use

```bash
mkdir NewRGB && mkdir NewSem
mkdir build && cd build && cmake .. && make -j4
../app/main <dataset path>
```

​	and new images in the NewRGB and NewSem folder.

​	At last , cp NewRGB -> image_2 ; NewSem->semantic and

```
python build_dataset.py --datasetPath <new-dataset-path> --outputPath <output-dataset> 
```

​	prefix for new images and semantics:

+ SD_X: SimpleDeformation
+ RS_: Resize
+ T_: Tailor
+ I _: Illumination
+ C_: Contrast
+ IC_:  Illumination and Contrast
+ S_: Saturation
+ M4_: Mosaic (refer to yolo v4)
+ RE_: RandomErasure (refer to yolo v4)

