import React from 'react';
import { Container, Row, Col } from 'react-bootstrap';
import SettingForm from './SettingForm';

const SettingPage = () => (
  <Container>
    <Row className="d-flex justify-content-center">
      <Col sm={12} md={6}>
        <SettingForm />
      </Col>
    </Row>
  </Container>
);

export default SettingPage;