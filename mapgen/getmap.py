import urllib.request
import os
import zipfile
import struct


def acquire_data_files(startLon, endLon, startLat, endLat):
    if not os.path.exists("data"):
        os.mkdir("data")

    _cnt = 0
    for i in range(startLat, endLat + 1):
        for j in range(startLon, endLon + 1):
            _cnt += 1
            print("Downloading " + str(_cnt) + "/" + str((endLon - startLon + 1) * (endLat - startLat + 1)))

            filename = "N{:02}E{:03}.hgt.zip".format(i, j)
            url = 'https://dds.cr.usgs.gov/srtm/version2_1/SRTM3/Eurasia/' + filename
            filepath = 'data/' + filename
            if not os.path.exists(filepath):
                urllib.request.urlretrieve(url, filepath)
                with zipfile.ZipFile(filepath, "r") as zip_to_extract:
                    zip_to_extract.extractall("data")


startLat = 45
endLat = 49
startLon = 16
endLon = 23
acquire_data_files(startLon, endLon, startLat, endLat)

width = endLon - startLon + 1
height = endLat - startLat + 1

hgt_width = 1201
hgt_height = 1201

print(width, height)
with open("data/merged.hgta", "wb") as out:
    for y in range(height):
        files_row = []
        for x in range(width):
            files_row.append(open("data/N{:02}E{:03}.hgt".format(height - y - 1 + startLat, x + startLon), "rb"))

        print(files_row)
        for yf in range(hgt_height):
            for x in range(width):
                for xf in range(hgt_width):
                    buf = files_row[x].read(2)
                    #Scaling down
                    if (x * hgt_width + xf) % 9 == 0 and (y * hgt_height + yf) % 9 == 0:
                        out.write(buf)


        for fr in files_row:
            fr.close()
