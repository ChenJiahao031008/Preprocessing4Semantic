import os
import argparse
import cv2

ap = argparse.ArgumentParser()
ap.add_argument('-d', '--dataset', required=True,
                help='Path to input dataset')
#ap.add_argument("-s", "--savepath", required=True,
#                 help="path to save dataset")
args = vars(ap.parse_args())

filePath = str(args['dataset'])
#savePath = args['savepath']

dirNumList = []
dirPathList = []

for dirname in os.listdir(filePath):
	if (dirname.find("_json")!=-1):
		dirPathList.append(dirname)
		dirname = dirname.replace( "_json","")
		dirNumList.append(dirname)

dirPathList.sort()
dirNumList.sort()

for i in range(len(dirPathList)):
	imagePath = filePath + "/" + str(dirPathList[i]) + "/img.png"
	semanticPath = filePath + "/" + str(dirPathList[i]) + "/label.png"
	imageOutput = filePath + "/image_2/" + str(dirNumList[i]) +".png"
	SemanticOutput = filePath + "/semantic/" + str(dirNumList[i]) +".png"

	image = cv2.imread(imagePath)
	semantic = cv2.imread(semanticPath)

	cv2.imwrite(imageOutput, image)
	cv2.imwrite(SemanticOutput, semantic)

