FROM debian:buster

RUN echo "- Updating Debian"
RUN apt-get update && apt-get upgrade -y

RUN echo "- Installing dependencies"
RUN apt-get install -y \
	gcc \
	g++ \
	ccache \
	cppcheck \
	doxygen \
	git \
	make \
	tar \
	unzip \
	cmake \
	libgtkmm-3.0 \
	libgtkmm-3.0-dev \
	libmariadb-dev

RUN echo "- Building the project"
RUN git clone https://github.com/cpwonline/modern-cxx-project.git
RUN cd modern-cxx-project && \
	mkdir build && cd build && \
	cmake .. && cmake --build . && \
	cmake --build . --target install

CMD /usr/local/bin/project-name