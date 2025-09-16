FROM fedora:42

RUN dnf update -y && dnf install make g++ git ip -y 
WORKDIR /app
RUN git clone https://github.com/BradyBangasser/porttorpedo
WORKDIR /app/porttorpedo

RUN make

COPY <<-EOF args.txt
127.0.0.1
EOF

COPY <<-EOF run.sh
/app/porttorpedo/porttorpedo < /app/porttorpedo/args.txt
EOF

ENTRYPOINT [ "sh", "./run.sh" ]
