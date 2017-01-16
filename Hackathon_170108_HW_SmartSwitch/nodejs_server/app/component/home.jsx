import React , {Component} from 'react';
import {Col , Row , Carousel } from 'antd';
const img1 = require('../src/HomeImage/1.png');
const img2 = require('../src/HomeImage/2.jpg');
export default class Home extends Component {
    constructor(props) {
        super(props);
        this.state = {}
    }

    render() {
        return (
            <div className="main-content">
                <Row>
                    <Col span={6}></Col>
                    <Col span={12}>
                        <Carousel autoplay>
                            <div>
                                <img src={img1} className="adImage"/>
                            </div>
                            <div>
                                <img src={img2} className="adImage"/>
                            </div>
                        </Carousel>
                    </Col>
                    <Col span={6}></Col>
                </Row>
            </div>
        )
    }
}
