# -*- coding: utf-8 -*-
import argparse
import random
import os
import cv2 as cv
# from PIL import Image
from tqdm import tqdm
import numpy as np

SIZE = [640,480]

# python build_dataset.py --datasetPath /home/demo/Data/Corn/dataset-all
parser = argparse.ArgumentParser()
parser.add_argument('--datasetPath', default='data', help="Directory with the SIGNS dataset")
parser.add_argument('--outputPath', default='preprocessed', help="Where to write the new data")


def resize_and_save_pil(filename, outputPath, size=SIZE):
    """Resize the image contained in `filename` and save it to the `outputPath`"""
    image = Image.open(filename)
    # Use bilinear interpolation instead of the default "nearest neighbor" method
    image = image.resize((size, size), Image.BILINEAR)
    image.save(os.path.join(outputPath, filename.split('/')[-1]))

def ResizeAndSave(filename, outputPath, size=SIZE,id=0):
    if (id==0):
        image = cv.imread(filename)
    else:
        image = cv.imread(filename, 0)
    #x, y = image.shape[0:2]
    image = cv.resize(image, (size[0], size[1]),interpolation=cv.INTER_AREA)
    #image = cv.resize(image, (int(x/2), int(y/2)),interpolation=cv.INTER_AREA)
    cv.imwrite(os.path.join(outputPath, filename.split('/')[-1]),image)

if __name__ == '__main__':
    args = parser.parse_args()
    # assert 表达式条件为 false 的时候触发异常。
    assert os.path.isdir(args.datasetPath), "[ERRO] Couldn't find the dataset at {}".format(args.datasetPath)

    # Define the data directories
    imagesDir = os.path.join(args.datasetPath, 'image_2')
    semanticsDir = os.path.join(args.datasetPath, 'semantic')

    # listdir:列出文件夹下所有文件。之后利用遍历找到所有.png后缀的图像
    imageNames = os.listdir(imagesDir) 
    imageNames = [os.path.join(imagesDir, f) for f in imageNames if f.endswith('.png')]
    sementicNames = os.listdir(semanticsDir)
    sementicNames = [os.path.join(semanticsDir, f) for f in sementicNames if f.endswith('.png')]

    # 由于数据集的语义和RGB图像尺寸大小相同，并且命名相同，所以如果数量一致的话，仅录入RGB图像的文件名即可
    assert len(imageNames)==len(sementicNames), "[ERRO] Dataset of images doesn't match that of annotation! "

    # .seed 生成随机数，种子号为230，且随机数固定
    random.seed(230)
    # 默认升序排序
    imageNames.sort()
    sementicNames.sort()
    # shuffle() 方法将序列的所有元素随机排序。
    state = np.random.get_state()
    np.random.shuffle(imageNames)
    np.random.set_state(state)
    np.random.shuffle(sementicNames)
    #random.shuffle(imageNames)
    #random.shuffle(sementicNames)

    # split1st： 分割 训练/验证集 和 测试集，比例为0.8:0.2
    split1st = int(0.9*len(imageNames))
    # split2nd： 分割 训练集 和 验证集，比例为0.8:0.2
    split2nd = int( split1st*0.05 )
    testImageNames  = imageNames[split1st:]
    valImageNames   = imageNames[:split2nd]
    trainImageNames = imageNames[split2nd:split1st]
    testSemanticNames  = sementicNames[split1st:]
    valSemanticNames   = sementicNames[:split2nd]
    trainSemanticNames = sementicNames[split2nd:split1st]
    # 建立字典，便于查询
    imageNames = {'train': trainImageNames,
                  'val': valImageNames,
                  'test': testImageNames} 
    semanticNames = {'train': trainSemanticNames,
                  'val': valSemanticNames,
                  'test': testSemanticNames}        

    if not os.path.exists(args.outputPath):
        os.mkdir(args.outputPath)
    else:
        print("[WARNING] Output dir {} already exists.".format(args.outputPath))

    # RGB图像数据：
    for split in ['train', 'val', 'test']:
        outputPath = os.path.join(args.outputPath, 'image')
        if not os.path.exists(outputPath):
            os.mkdir(outputPath)
        outputPathSplit = os.path.join(outputPath, '{}'.format(split))
        if not os.path.exists(outputPathSplit):
            os.mkdir(outputPathSplit)
        else:
            print("[WARNING] Dir {} already exists.".format(outputPathSplit))

        print("[INFO] Processing {} data, saving preprocessed data to {}".format(split, outputPathSplit))
        # tqdm 为进度条模块
        for filename in tqdm(imageNames[split]):
            ResizeAndSave(filename, outputPathSplit, size=SIZE, id=0)
    # 语义图像
    for split in ['train', 'val', 'test']:
        outputPath = os.path.join(args.outputPath, 'semantic')
        if not os.path.exists(outputPath):
            os.mkdir(outputPath)
        outputPathSplit = os.path.join(outputPath, '{}'.format(split))
        if not os.path.exists(outputPathSplit):
            os.mkdir(outputPathSplit)
        else:
            print("[WARNING] Dir {} already exists.".format(outputPathSplit))

        print("[INFO] Processing {} data, saving preprocessed data to {}".format(split, outputPathSplit))
        # tqdm 为进度条模块
        for filename in tqdm(semanticNames[split]):
            ResizeAndSave(filename, outputPathSplit, size=SIZE, id=0)

    print("[INFO] Done building dataset")
