import React, {useState} from 'react';
import { Carousel, Jumbotron } from 'react-bootstrap';

const Demo = () => {
  const [activeIndex, setActiveIndex] = useState(0);

  const handleSelect = (selectedIndex, e) => {
    setActiveIndex(selectedIndex);
  };

  return (
    <Carousel activeIndex={activeIndex} onSelect={handleSelect}>
      <Carousel.Item>
        <img
          className="d-block w-100"
          src="/img/ustc.jpg"
          alt="First slide"
        />
        <Carousel.Caption className="text-dark">
          <h3 className="animate__animated animate__fadeInDown">USTC</h3>
          <p className="animate__animated animate__fadeInUp">
            University of Science and Technology of China
          </p>
        </Carousel.Caption>
      </Carousel.Item>
      <Carousel.Item>
        <img
          className="d-block w-100"
          src="/img/teach.png"
          alt="Second slide"
        />
        <Carousel.Caption className="text-dark">
          <h3 className="animate__animated animate__fadeInDown">教学</h3>
          <p className="animate__animated animate__fadeInUp">
            Teaching and Learning
          </p>
        </Carousel.Caption>
      </Carousel.Item>
      <Carousel.Item>
        <img
          className="d-block w-100"
          src="/img/research.png"
          alt="Third slide"
        />
        <Carousel.Caption className="text-dark">
          <h3 className="animate__animated animate__fadeInDown">科研</h3>
          <p className="animate__animated animate__fadeInUp">
            Research and Innovation
          </p>
        </Carousel.Caption>
      </Carousel.Item>
    </Carousel>
  );
};

const Slogan = () => (
  <Jumbotron className="flex-fill m-0 d-flex align-items-center">
    <div className="text-center">
      <h1 className="mb-4">USTC 教学科研登记系统</h1>
      <p className="lead">
        欢迎来到 USTC 教学科研登记系统!
      </p>
    </div>
  </Jumbotron>
);
  

  const Introduction = () => (
    <div>
      <Demo />
      <Slogan />
    </div>
  );

export {Demo, Slogan};
export default Introduction;