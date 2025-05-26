EXEC = app.out

main:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make && \
	mv ${EXEC} ..

clean:
	rm -rf build/ ${EXEC}