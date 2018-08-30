#!/usr/bin/env python

import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate master description from Wavefront file')
    parser.add_argument("file", help="Wavefront file to convert")
    parser.add_argument("-v", "--verbose", action="store_true", help="increase verbosity")
    args = parser.parse_args()

    if args.verbose:
        print("Processing file: {}".format(args.file))

    vertices = []
    faces = []

    # Read vertex and face data from Wavefront file
    with open(args.file, 'r') as f:
        for line in f:
            data = line.split()

            if not data:
                continue

            if data[0] == "v":
                vertices.append((data[1], data[2], data[3]))
            elif data[0] == "f":
                faces.append((data[1].split('/')[0], data[2].split('/')[0], data[3].split('/')[0]))

        if args.verbose:
            print("Number of vertices: {}".format(len(vertices)))
            print("Number of faces: {}".format(len(faces)))

    # Now output converted 
    print("master_no = 1")
    print(".objectdef")

    print("\tno_vertices = {}".format(len(vertices)))
    for i,v in enumerate(vertices):
        print("\t\t{} = {} , {} , {}".format(i+1, v[0], v[1], v[2]))

    print("\nno_polygons = {}".format(len(faces)))
    for i,f in enumerate(faces):
        print("\t\t{} = {} , {} , {}".format(i+1, f[0], f[1], f[2]))

    print(".objectend")
    print("\tscale = 1.0 , 1.0 , 1.0")
    print
    
