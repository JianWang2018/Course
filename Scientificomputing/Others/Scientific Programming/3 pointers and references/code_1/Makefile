
all: ref.x pointer.x obj_ref_size.x

ref.x: reference.cpp
	g++ -o $@ $<

pointer.x: pointer.cpp
	g++ -o $@ $<

obj_ref_size.x: object_reference_size.cpp
	g++ -o $@ $<

clean:
	rm *.x
