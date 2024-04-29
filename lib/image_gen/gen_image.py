from PIL import Image


def print_rgb_hex_values(image_path, vga_opt):
    """Prints the RGB values of each pixel in an image as hex values."""
    if vga_opt == 0:
        with Image.open(image_path) as image:
            x = image.size[0]
            y = image.size[1]
            pixels = image.load()  # Load the pixel data
            print("{", end="")
            for line in range(y):
                #print("{", end="")
                for pix in range(x):
                    r, g, b, a = pixels[pix, line]
                    hex_value = f"0x00{r:02x}{g:02x}{b:02x}"
                    print(hex_value, end=", ")
                print("")
                    #print(hex_value, end=f"{', '}") # if pix != x - 1 else ''}")
                #print("},", end="\n") if line != y - 1 else print("}", end="")
            print("};")


image_in = "nice_land.png"  # input("File name (png): ")
vga_mode = int(input("VGA Mode: "))

print_rgb_hex_values(image_in, vga_mode)
