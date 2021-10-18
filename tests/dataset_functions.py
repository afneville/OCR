from matplotlib import pyplot as plt
import numpy as np
import json

def convert_type(string):
    return float(string)

def get_info(path):

    with open (path, "r") as info:
        data = json.load(info)
    return data

def get_datasets(data, dataset_path, sample_path):

    training_dataset = []
    with open(dataset_path, "r") as dataset:
        for character_num in range(len(data["characters"])):
            dataset.seek(data["characters"][character_num]["position"] * 10 * 8 * 32 * 32)
            for sample in range(10):
                current_character = []
                for row_string in range(32):
                    row_string = dataset.read(8*32)[:-1]
                    row_pixels = row_string.split(",")
                    row_pixels = list(map(convert_type, row_pixels))
                    current_character.append(row_pixels)
                training_dataset.append(np.array(current_character))

    sample_dataset = []
    with open(sample_path, "r") as dataset:
        for character_num in range(len(data["characters"])):
            dataset.seek(data["characters"][character_num]["position"] * 8 * 32 * 32)
            current_character = []
            for row in range(32):
                row_string = dataset.read(8*32)[:-1]
                row_pixels = row_string.split(",")
                row_pixels = list(map(convert_type, row_pixels))
                current_character.append(row_pixels)

            sample_dataset.append(np.array(current_character))

    return training_dataset, sample_dataset

def save_numbers(characters):
    for i in range(len(characters)):
        characters[i] = characters[i].astype(int)
        plt.imsave("./output/numbers/"+str(i)+".png", characters[i], cmap=plt.get_cmap('gray'))

def save_alphabet(characters):
    for i in range(len(characters)):
        characters[i] = characters[i].astype(int)
        labels = [i for i in "abcdefghijklmnopqrstuvwxyz"]
        plt.imsave("./output/alphabet/"+labels[i]+".png", characters[i], cmap=plt.get_cmap('gray'))

def invert_colours(samples):
    threshold = np.full((32,32), 255.0)
    for i in range(len(samples)):
        samples[i] = np.subtract(threshold, samples[i])
